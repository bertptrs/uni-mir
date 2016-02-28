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
?>
<!DOCTYPE HTML>
<html>
<head>
<title>MIRgle - Search for Multimedia information retrieval by Bert Peters</title>
</head>
<body>

<form>
    <h1><span style="color:#4285f4;">M</span><span style="color:#ea4335;">I</span><span style="color:#ffbc05">R</span><span style="color:#4285f4;">g</span><span style="color:#34a853;">l</span><span style="color:#ffbc05">e</span></h1>
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
}
?>
</body>
</html>
