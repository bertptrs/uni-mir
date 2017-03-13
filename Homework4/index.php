#!/usr/bin/php5-cgi
<!DOCTYPE HTML>
<html>
<head>
<title>MIRgle - Search for Multimedia information retrieval by Bert Peters</title>
<style>
body {
    font-family: sans-serif;
}

h2 {
    font-size: 18px;
    text-overflow: ellipsis;
    white-space: nowrap;
    display: block;
    margin: 2px 0;
}

div.url {
    color: #006621;
}

p {
    color: #545454;
    margin-top: 2px;
    margin-bottom: 10px;
}

</style>
</head>
<body>

<form>
    <h1><span style="color:#4285f4;">M</span><span style="color:#ea4335;">I</span><span style="color:#ffbc05">R</span><span style="color:#4285f4;">g</span><span style="color:#34a853;">l</span><span style="color:#ffbc05">e</span></h1>
    <input type="search" name="q" value="<?= isset($_GET['q']) ? htmlentities($_GET['q']) : "" ?>">
    <button type="submit" name="search">
        Search
    </button>
    <button type="submit" name="image">
        Image search
    </button>
</form>
<hr>
<?php
if (isset($_GET['q'])) {
    if (isset($_GET['search'])) {
        require 'search.php';
    } elseif (isset($_GET['image'])) {
        echo "TODO";
    }
}
?>
</body>
</html>
