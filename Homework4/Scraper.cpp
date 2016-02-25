#include "Scraper.hpp"
#include "LinkHelper.hpp"
#include "htmlstreamparser.h"

void Scraper::load(const std::string& newURL)
{
	data = client.getURL(newURL);
	url = newURL;
}

std::vector<std::string> Scraper::getWeblinks() const
{
	return getLinksByTagName("a", "href");
}


std::vector<std::string> Scraper::getImagelinks() const
{
	return getLinksByTagName("img", "src");
}

std::vector<std::string> Scraper::getLinksByTagName(const char* tag_name, const char* attribute) const
{
	// Buffers for the stream parser.
	char tag[8];
	char attr[20];
	char val[1024];
	// Links string.
	std::vector<std::string> links;
	std::string base = url;

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
					std::string link = LinkHelper::relativize(html_parser_val(hsp), base);
					links.push_back(link);
				}
			}
		}
	}

	/*release the hsp*/
	html_parser_cleanup(hsp);
	return links;
}
