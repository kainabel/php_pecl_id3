--TEST--
Test set ID3 functionality
--INI--
xdebug.default_enable=0
--FILE--
<?php

function openFile() {
    return dirname(__FILE__) . "/test.set.mp3";
}

function openFileStream() {
    return fopen(openFile(), 'r');
}

function printTagVersion($path) {
    $version = id3_get_version($path);
    if ($version & ID3_V1_0) {
        echo "Contains a 1.x tag\n";
    }
    if ($version & ID3_V1_1) {
        echo "Contains a 1.1 tag\n";
    }
    if ($version & ID3_V2_1) {
        echo "Contains a 2.x tag\n";
    }
    if ($version & ID3_V2_2) {
        echo "Contains a 2.2 tag\n";
    }
    if ($version & ID3_V2_3) {
        echo "Contains a 2.3 tag\n";
    }
    if ($version & ID3_V2_4) {
        echo "Contains a 2.4 tag\n";
    }
}

var_dump(id3_remove_tag(openFile(), ID3_V1_0));

printTagVersion(openFile());

var_dump(id3_set_tag(openFile(), array("title" => "Re:Start", "artist" => "Re:\Legion", "comment" => "A nice track"), ID3_V1_1));

var_dump(id3_get_tag(openFile()));

var_dump(id3_get_tag(openFile(), ID3_V1_0));

printTagVersion(openFile());

var_dump(id3_remove_tag(openFile(), ID3_V1_0));

printTagVersion(openFile());

var_dump(id3_remove_tag(openFileStream(), ID3_V1_0));

printTagVersion(openFileStream());

var_dump(id3_set_tag(openFileStream(), array("title" => "Re:Start", "artist" => "Re:\Legion", "comment" => "A nice track"), ID3_V1_1));

var_dump(id3_get_tag(openFileStream()));

var_dump(id3_get_tag(openFileStream(), ID3_V1_0));

printTagVersion(openFileStream());

var_dump(id3_remove_tag(openFileStream(), ID3_V1_0));

printTagVersion(openFileStream());

?>
--EXPECTF--
Notice: id3_remove_tag(): id3_remove_tag() no ID3v1 tag found in %s on line %d
bool(false)
Contains a 2.x tag
Contains a 2.2 tag
Contains a 2.3 tag
Contains a 2.4 tag
bool(true)
array(7) {
  ["genre"]=>
  string(4) "Acid"
  ["track"]=>
  string(4) "2016"
  ["album"]=>
  string(10) "Test Album"
  ["title"]=>
  string(10) "Test Track"
  ["year"]=>
  string(4) "2016"
  ["recTime"]=>
  string(4) "2016"
  ["artist"]=>
  string(11) "Test Artist"
}
array(5) {
  ["title"]=>
  string(8) "Re:Start"
  ["artist"]=>
  string(10) "Re:\Legion"
  ["album"]=>
  string(0) ""
  ["comment"]=>
  string(12) "A nice track"
  ["genre"]=>
  int(0)
}
Contains a 1.x tag
Contains a 1.1 tag
Contains a 2.x tag
Contains a 2.2 tag
Contains a 2.3 tag
Contains a 2.4 tag
bool(true)
Contains a 2.x tag
Contains a 2.2 tag
Contains a 2.3 tag
Contains a 2.4 tag

Notice: id3_remove_tag(): id3_remove_tag() no ID3v1 tag found in %s on line %d
bool(false)
Contains a 2.x tag
Contains a 2.2 tag
Contains a 2.3 tag
Contains a 2.4 tag
bool(true)
array(7) {
  ["genre"]=>
  string(4) "Acid"
  ["track"]=>
  string(4) "2016"
  ["album"]=>
  string(10) "Test Album"
  ["title"]=>
  string(10) "Test Track"
  ["year"]=>
  string(4) "2016"
  ["recTime"]=>
  string(4) "2016"
  ["artist"]=>
  string(11) "Test Artist"
}

Warning: id3_get_tag(): id3_get_tag() specified tag-version not available - try ID3_BEST in %s on line %d
array(0) {
}
Contains a 2.x tag
Contains a 2.2 tag
Contains a 2.3 tag
Contains a 2.4 tag

Notice: id3_remove_tag(): id3_remove_tag() no ID3v1 tag found in %s on line %d
bool(false)
Contains a 2.x tag
Contains a 2.2 tag
Contains a 2.3 tag
Contains a 2.4 tag
