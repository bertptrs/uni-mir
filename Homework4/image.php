<?php

$results = explode("\n", `./webquery --image`);

foreach ($results as $result)
{
    // If we accidentally have a relative URL, skip.
    if (strpos($result, ':') === false) {
        continue;
    }
?>
    <div class="img-result" style="background-image: url(<?= htmlentities($result) ?>);">
<a href="<?= htmlentities($result) ?>">&nbsp;</a>
</div>
<?php
}
