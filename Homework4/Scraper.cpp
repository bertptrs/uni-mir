#include <memory>
#include "Scraper.hpp"
#include "LinkHelper.hpp"
#include "htmlstreamparser.h"

using namespace std;

string Scraper::load(const string& newURL)
{
	url = newURL;
	data = client.getURL(url);

	return url;
}

vector<string> Scraper::getWeblinks() const
{
	return getLinksByTagName("a", "href");
}


vector<string> Scraper::getImagelinks() const
{
	return getLinksByTagName("img", "src");
}

vector<string> Scraper::getLinksByTagName(const char* tag_name, const char* attribute) const
{
	// Buffers for the stream parser.
	char tag[8];
	char attr[20];
	char val[1024];
	// Links string.
	vector<string> links;
	string base = url;

	// Initialize the parser
	HTMLSTREAMPARSER *hsp = html_parser_init( );
	html_parser_set_tag_to_lower(hsp, 1);
	html_parser_set_attr_to_lower(hsp, 1);
	html_parser_set_tag_buffer(hsp, tag, sizeof(tag));
	html_parser_set_attr_buffer(hsp, attr, sizeof(attr));
	html_parser_set_val_buffer(hsp, val, sizeof(val)-1);

	const size_t tag_length = strlen(tag_name);
	const size_t attr_length = strlen(attribute);

	// Loop over document
	for (char c : data) {
		html_parser_char_parse(hsp, c);
		// Detect different html base
		if (html_parser_cmp_tag(hsp, "base", 4) && html_parser_cmp_attr(hsp, "href", 4) && html_parser_is_in(hsp, HTML_VALUE_ENDED)) {
			html_parser_val(hsp)[html_parser_val_length(hsp)] = '\0';
			base = html_parser_val(hsp);
		}
		if (html_parser_cmp_tag(hsp, tag_name, tag_length)) {
			if (html_parser_cmp_attr(hsp, attribute, attr_length)) {
				if (html_parser_is_in(hsp, HTML_VALUE_ENDED)) {
					html_parser_val(hsp)[html_parser_val_length(hsp)] = '\0';
					string link = LinkHelper::relativize(html_parser_val(hsp), base);
					links.push_back(link);
				}
			}
		}
	}

	/*release the hsp*/
	html_parser_cleanup(hsp);
	return links;
}

const string& Scraper::getData() const
{
	return data;
}

unordered_set<string> Scraper::getWords() const
{
	return getWordsFromTag("body");
}

unordered_set<string> Scraper::getTitleWords() const
{
	return getWordsFromTag("title");
}

unordered_set<string> Scraper::getWordsFromTag(const string& tag_name) const
{
	stringstream contents;

	HTMLSTREAMPARSER *hsp = html_parser_init( );
	char tag[20];
	char attr[100];
	char val[128];
	html_parser_set_tag_to_lower(hsp, 1);
	html_parser_set_attr_to_lower(hsp, 1);
	html_parser_set_tag_buffer(hsp, tag, sizeof(tag));
	html_parser_set_attr_buffer(hsp, attr, sizeof(attr));
	html_parser_set_val_buffer(hsp, val, sizeof(val)-1);

	unique_ptr<char> innerBuffer(new char[data.size()]);
	html_parser_set_inner_text_buffer(hsp, innerBuffer.get(), data.size() - 1);

	const auto closeTag = "/" + tag_name;
	bool inTag = false;


	for (char c : data) {
		html_parser_char_parse(hsp, c);/*Parse the char specified by the char argument*/
		if (inTag) {
			if (html_parser_is_in(hsp, HTML_TAG_BEGINNING)) {
				// Write current inner text to buffer.
				char* innerText = html_parser_inner_text(hsp);
				innerText[html_parser_inner_text_length(hsp)] = '\0';
				for (size_t i = 0; i < html_parser_inner_text_length(hsp); i++) {
					contents << (char) tolower(innerText[i]);
				}
			}
			if (html_parser_cmp_tag(hsp, closeTag.c_str(), closeTag.size())) {
				// reached end of requested tag, can finish now.
				break;
			}
		} else {
			if (html_parser_cmp_tag(hsp, tag_name.c_str(), tag_name.size())) {
				inTag = true;
			}
		}
	}

	html_parser_cleanup(hsp);

	unordered_set<string> words;
	string word;
	while (contents) {
		contents >> word;
		if (!word.empty()) {
			words.insert(word);
		}
	}

	return words;
}
