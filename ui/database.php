<?php
// Content of foundation database info
$mysqli = new mysqli('localhost', 'jiemiao', 'pass', 'fishing');
if($mysqli->connect_errno) {
    printf("Connection Failed: %s\n", $mysqli->connect_error);
    exit;
}
?>
