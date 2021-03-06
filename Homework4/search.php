<?php
function getTitle($filename) {
    $document = new DOMDocument();
    @$document->loadHTMLFile($filename);
    $titles = $document->getElementsByTagName("title");

    if ($titles->length == 0) {
        return false;
    } else {
        return $titles->item(0)->textContent;
    }
}

function getSnippet($filename) {
    $document = new DOMDocument();
    @$document->loadHTMLFile($filename);
    $bodys = $document->getElementsByTagName("p");
    for ($i = 0; $i < $bodys->length; $i++) {
        $content = trim(str_replace("&nbsp;", " ", $bodys->item($i)->textContent), " \t\n\r\0\x0B\xA0\xC2");
        if (strlen($content) > 0) {
            return preg_replace('/\s+/', ' ', $content);
        }
    }
    return "";
}
$results = explode("\n", `./webquery`);
foreach ($results as $url) {
    if (strlen($url) == 0) {
        continue;
    }
    $filename = "data/repository/" . md5($url);
    $title = getTitle($filename);
    $snippet = getSnippet($filename);
?>
<div class="result">
    <h2><a href="<?= htmlentities($url) ?>"><?= $title ? $title : "No title" ?></a></h2>
    <div class="url"><?= htmlentities($url) ?></div>
    <p><?= htmlentities($snippet) ?></p>
</div>
<?php
}
