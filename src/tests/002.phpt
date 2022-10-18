--TEST--
dump
--EXTENSIONS--
example
--FILE--
<?php
$string = "hello world";
dump($string);
?>
--EXPECT--
STRING: value="hello world", length=11