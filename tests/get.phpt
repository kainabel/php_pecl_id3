--TEST--
Test get ID3 functionality
--INI--
xdebug.default_enable=0
--FILE--
<?php

function openFile() {
    return dirname(__FILE__) . "/test.mp3";
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

var_dump(id3_get_tag(openFile()));

var_dump(id3_get_tag(openFile(), ID3_V1_0));

printTagVersion(openFile());

var_dump(id3_get_tag(openFileStream()));

var_dump(id3_get_tag(openFileStream(), ID3_V1_0));

printTagVersion(openFileStream());

var_dump(id3_get_frame_long_name("TOLY"));

var_dump(id3_get_frame_short_name("TOLY"));

var_dump(id3_get_genre_name(20) === id3_get_genre_id("Alternative"));

var_dump(id3_get_genre_list());

?>
--EXPECTF--
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
string(35) "Original lyricist(s)/text writer(s)"
string(16) "originalLyricist"
bool(false)
array(148) {
  [0]=>
  string(5) "Blues"
  [1]=>
  string(12) "Classic Rock"
  [2]=>
  string(7) "Country"
  [3]=>
  string(5) "Dance"
  [4]=>
  string(5) "Disco"
  [5]=>
  string(4) "Funk"
  [6]=>
  string(6) "Grunge"
  [7]=>
  string(7) "Hip-Hop"
  [8]=>
  string(4) "Jazz"
  [9]=>
  string(5) "Metal"
  [10]=>
  string(7) "New Age"
  [11]=>
  string(6) "Oldies"
  [12]=>
  string(5) "Other"
  [13]=>
  string(3) "Pop"
  [14]=>
  string(3) "R&B"
  [15]=>
  string(3) "Rap"
  [16]=>
  string(6) "Reggae"
  [17]=>
  string(4) "Rock"
  [18]=>
  string(6) "Techno"
  [19]=>
  string(10) "Industrial"
  [20]=>
  string(11) "Alternative"
  [21]=>
  string(3) "Ska"
  [22]=>
  string(11) "Death Metal"
  [23]=>
  string(6) "Pranks"
  [24]=>
  string(10) "Soundtrack"
  [25]=>
  string(11) "Euro-Techno"
  [26]=>
  string(7) "Ambient"
  [27]=>
  string(8) "Trip-Hop"
  [28]=>
  string(5) "Vocal"
  [29]=>
  string(9) "Jazz+Funk"
  [30]=>
  string(6) "Fusion"
  [31]=>
  string(6) "Trance"
  [32]=>
  string(9) "Classical"
  [33]=>
  string(12) "Instrumental"
  [34]=>
  string(4) "Acid"
  [35]=>
  string(5) "House"
  [36]=>
  string(4) "Game"
  [37]=>
  string(10) "Sound Clip"
  [38]=>
  string(6) "Gospel"
  [39]=>
  string(5) "Noise"
  [40]=>
  string(16) "Alternative Rock"
  [41]=>
  string(4) "Bass"
  [42]=>
  string(4) "Soul"
  [43]=>
  string(4) "Punk"
  [44]=>
  string(5) "Space"
  [45]=>
  string(10) "Meditative"
  [46]=>
  string(16) "Instrumental Pop"
  [47]=>
  string(17) "Instrumental Rock"
  [48]=>
  string(6) "Ethnic"
  [49]=>
  string(6) "Gothic"
  [50]=>
  string(8) "Darkwave"
  [51]=>
  string(17) "Techno-Industrial"
  [52]=>
  string(10) "Electronic"
  [53]=>
  string(8) "Pop-Folk"
  [54]=>
  string(9) "Eurodance"
  [55]=>
  string(5) "Dream"
  [56]=>
  string(13) "Southern Rock"
  [57]=>
  string(6) "Comedy"
  [58]=>
  string(4) "Cult"
  [59]=>
  string(7) "Gangsta"
  [60]=>
  string(6) "Top 40"
  [61]=>
  string(13) "Christian Rap"
  [62]=>
  string(8) "Pop/Funk"
  [63]=>
  string(6) "Jungle"
  [64]=>
  string(9) "Native US"
  [65]=>
  string(7) "Cabaret"
  [66]=>
  string(8) "New Wave"
  [67]=>
  string(11) "Psychedelic"
  [68]=>
  string(4) "Rave"
  [69]=>
  string(9) "Showtunes"
  [70]=>
  string(7) "Trailer"
  [71]=>
  string(5) "Lo-Fi"
  [72]=>
  string(6) "Tribal"
  [73]=>
  string(9) "Acid Punk"
  [74]=>
  string(9) "Acid Jazz"
  [75]=>
  string(5) "Polka"
  [76]=>
  string(5) "Retro"
  [77]=>
  string(7) "Musical"
  [78]=>
  string(11) "Rock & Roll"
  [79]=>
  string(9) "Hard Rock"
  [80]=>
  string(4) "Folk"
  [81]=>
  string(9) "Folk-Rock"
  [82]=>
  string(13) "National Folk"
  [83]=>
  string(5) "Swing"
  [84]=>
  string(11) "Fast Fusion"
  [85]=>
  string(5) "Bebob"
  [86]=>
  string(5) "Latin"
  [87]=>
  string(7) "Revival"
  [88]=>
  string(6) "Celtic"
  [89]=>
  string(9) "Bluegrass"
  [90]=>
  string(10) "Avantgarde"
  [91]=>
  string(11) "Gothic Rock"
  [92]=>
  string(16) "Progressive Rock"
  [93]=>
  string(16) "Psychedelic Rock"
  [94]=>
  string(14) "Symphonic Rock"
  [95]=>
  string(9) "Slow Rock"
  [96]=>
  string(8) "Big Band"
  [97]=>
  string(6) "Chorus"
  [98]=>
  string(14) "Easy Listening"
  [99]=>
  string(8) "Acoustic"
  [100]=>
  string(6) "Humour"
  [101]=>
  string(6) "Speech"
  [102]=>
  string(7) "Chanson"
  [103]=>
  string(5) "Opera"
  [104]=>
  string(13) "Chamber Music"
  [105]=>
  string(6) "Sonata"
  [106]=>
  string(8) "Symphony"
  [107]=>
  string(10) "Booty Bass"
  [108]=>
  string(6) "Primus"
  [109]=>
  string(11) "Porn Groove"
  [110]=>
  string(6) "Satire"
  [111]=>
  string(8) "Slow Jam"
  [112]=>
  string(4) "Club"
  [113]=>
  string(5) "Tango"
  [114]=>
  string(5) "Samba"
  [115]=>
  string(8) "Folklore"
  [116]=>
  string(6) "Ballad"
  [117]=>
  string(12) "Power Ballad"
  [118]=>
  string(12) "Rhytmic Soul"
  [119]=>
  string(9) "Freestyle"
  [120]=>
  string(4) "Duet"
  [121]=>
  string(9) "Punk Rock"
  [122]=>
  string(9) "Drum Solo"
  [123]=>
  string(8) "Acapella"
  [124]=>
  string(10) "Euro-House"
  [125]=>
  string(10) "Dance Hall"
  [126]=>
  string(3) "Goa"
  [127]=>
  string(11) "Drum & Bass"
  [128]=>
  string(10) "Club-House"
  [129]=>
  string(8) "Hardcore"
  [130]=>
  string(6) "Terror"
  [131]=>
  string(5) "Indie"
  [132]=>
  string(7) "BritPop"
  [133]=>
  string(9) "Negerpunk"
  [134]=>
  string(10) "Polsk Punk"
  [135]=>
  string(4) "Beat"
  [136]=>
  string(17) "Christian Gangsta"
  [137]=>
  string(11) "Heavy Metal"
  [138]=>
  string(11) "Black Metal"
  [139]=>
  string(9) "Crossover"
  [140]=>
  string(14) "Contemporary C"
  [141]=>
  string(14) "Christian Rock"
  [142]=>
  string(8) "Merengue"
  [143]=>
  string(5) "Salsa"
  [144]=>
  string(12) "Thrash Metal"
  [145]=>
  string(5) "Anime"
  [146]=>
  string(4) "JPop"
  [147]=>
  string(8) "SynthPop"
}
