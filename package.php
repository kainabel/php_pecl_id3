<?php
/**
 * script to automate the generation of the
 * package.xml file.
 *
 * $Id$
 *
 * @author      Stephan Schmidt <schst@php-tools.net>
 * @package     id3
 * @subpackage  Tools
 */

/**
 * uses PackageFileManager
 */ 
require_once 'PEAR/PackageFileManager.php';

/**
 * current version
 */
$version = '0.2';

/**
 * current state
 */
$state = 'alpha';

/**
 * release notes
 */
$notes = <<<EOT
added support to read id3v2 text- and url-frames
EOT;

/**
 * package description
 */
$description = <<<EOT
id3 enables to to retrieve and update information from ID3 tags in
MP3 files. It supports version 1.0, 1.1 and 2.2+ (only reading text- and url-frames at the moment).
EOT;

$package = new PEAR_PackageFileManager();

$result = $package->setOptions(array(
    'package'           => 'id3',
    'summary'           => 'Functions to read and write ID3 tags in MP3 files.',
    'description'       => $description,
    'version'           => $version,
    'state'             => $state,
    'license'           => 'PHP License',
    'filelistgenerator' => 'cvs',
    'ignore'            => array('package.php', 'package.xml'),
    'notes'             => $notes,
    'simpleoutput'      => true,
    'baseinstalldir'    => '',
    'packagedirectory'  => './',
    'dir_roles'         => array('docs' => 'doc',
                                 'examples' => 'doc',
                                 'tests' => 'test',
                                 ),
    ));

if (PEAR::isError($result)) {
    echo $result->getMessage();
    die();
}
$package->addRole ('c', 'src');
$package->addRole ('h', 'src');
$package->addRole ('m4', 'src');
$package->addRole ('w32', 'src');
$package->addConfigureOption ('enable-id3', 'Enable id3 support?', 'autodetect');
$package->addMaintainer('schst', 'lead', 'Stephan Schmidt', 'schst@php-tools.net');
$package->addMaintainer('luckec', 'lead', 'Carsten Lucke', 'luckec@php.net');
$package->addDependency('php', '4.3.0', 'ge', 'php', false);

if (isset($_GET['make']) || (isset($_SERVER['argv'][1]) && $_SERVER['argv'][1] == 'make')) {
    $result = $package->writePackageFile();
} else {
    $result = $package->debugPackageFile();
}

if (PEAR::isError($result)) {
    echo $result->getMessage();
    die();
}
?>
