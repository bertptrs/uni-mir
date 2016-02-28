<?php

function getTitle($filename) {
    $document = new DOMDocument();
    @$document->loadHTMLFile($filename);
    $titles = $document->getElementsByTagName("title");

    if ($titles->length == 0) {
        return $filename;
    } else {
        return $titles->item(0)->textContent;
    }
}

function getSnippet($filename) {
    $document = new DOMDocument();
    @$document->loadHTMLFile($filename);
    $bodys = $document->getElementsByTagName("body");
    if ($bodys->length == 0) {
        return "";
    } else {
        $content = $bodys->item(0)->textContent;

        return preg_replace('/\s+/', ' ', $content);
    }
}
?>
<!DOCTYPE HTML>
<html>
<head>
<title>MIRgle - Search for Multimedia information retrieval by Bert Peters</title>
</head>
<body>

<form>
    <h1>MIRgle</h1>
    <input type="search" name="q" value="<?= isset($_GET['q']) ? htmlentities($_GET['q']) : "" ?>">
    <button type="submit">
        Search
    </button>
</form>
<hr>
<?php
if (isset($_GET['q'])) {
    file_put_contents("query.txt", $_GET['q']);
    $results = explode("\n", `./webquery`);
    foreach ($results as $url) {
        $filename = "data/repository/" . md5($url);
        $title = getTitle($filename);
        $snippet = getSnippet($filename);
?>
<div class="result">
    <h2><a href="<?= htmlentities($url) ?>"><?= $title ?></a></h2>
    <div class="url"><?= htmlentities($url) ?></div>
    <p><?= htmlentities($snippet) ?></p>
</div>
<?php
    }
}
?>
</body>
</html>
