<?php

require 'config.php';
require 'vendor/autoload.php';

$types = array(
    "name"        => "Package name",
    "keyword"     => "Package keywords",
    "description" => "Package description",
);

function splitTerms($data)
{
    return preg_split('/[^a-z0-9]+/i', $data, -1, PREG_SPLIT_NO_EMPTY);
}

function searchResults()
{
    $query = array(
        'conjunctive' => false,
        'terms'       => array(),
    );

    global $types;

    foreach ($types as $type => $_) {
        foreach (splitTerms($_GET[$type]) as $term) {
            $query['terms'][] = array(
                'type' => $type,
                'word' => strtolower($term),
            );
        }
    }

    $queryFile = tempnam(sys_get_temp_dir(), 'search_');
    file_put_contents($queryFile, json_encode($query));

    $results = array();
    $command = EXE_DIR . '/search ' . escapeshellarg($queryFile) . ' ' . DATA_DIR;
    exec($command, $results);
    unlink($queryFile);

    return $results;
}

function packageInfo($package)
{
    $filename = DATA_DIR . '/0/' . md5($package);

    $data = json_decode(file_get_contents($filename), true);

    $versions = \Composer\Semver\Semver::rsort(array_keys($data['packages'][$package]));

    $index = 0;
    for ($i = 0; $i < count($versions); $i++) {
        // Try to show current stable version
        if (\Composer\Semver\VersionParser::parseStability($versions[$i]) == 'stable') {
            $index = $i;
            break;
        }
    }

    return array(
        'version' => $versions[$index],
        'data'    => $data['packages'][$package][$versions[$index]],
    );
}

if (isset($_GET['name'])) {
    $results = searchResults();
}

?>
<!DOCTYPE HTML>
<html>
<head>
    <title>Packasearch</title>
    <link rel="stylesheet" href="vendor/twbs/bootstrap/dist/css/bootstrap.css">
</head>
<body>
<div class="container">
    <h1>Packasearch
        <small>Better search in Packagist</small>
    </h1>
    <form>
        <?php

        foreach ($types as $key => $desc) {
            ?>
            <div class="form-group">
                <input class="form-control" name="<?php echo $key; ?>" placeholder="<?php echo $desc; ?>"
                       value="<?php if (isset($_GET[$key])) {
                           echo htmlentities($_GET[$key]);
                       } ?>">
            </div>
            <?php
        }
        ?>
        <button class="btn btn-primary" type="submit">
            <span class="glyphicon glyphicon-search"></span> Search
        </button>
    </form>

    <hr>
    <?php
    if (isset($results)) {
        echo '<h2>Results</h2>';;
        if (count($results) > 0) {
            $i = 0;
            foreach ($results as $result) {
                ?>
                <div class="result">
                    <h3><a href="https://packagist.org/packages/<?php echo $result; ?>"><?php echo $result; ?></a></h3>
                    <?php
                    $data = packageInfo($result);
                    echo '<p>Version: ' . $data['version'] . '</p>';
                    echo '<p>' . $data['data']['description'] . '</p>';
                    ?>
                </div>
                <?php
                if (++$i >= RESULT_LIMIT) {
                    break;
                }
            }
            echo '<hr><p>' . $i . ' of ' . count($results) . ' shown.</p>';
        } else {
            echo '<p>No results found.</p>';
        }
    }
    ?>
</div>
</body>
</html>