<?php
// destory the session so that the user info is secure
require 'database.php';
header("Content-Type: application/json"); // Since we are sending a JSON response here (not an HTML document), set the MIME Type to application/json
ini_set("session.cookie_httponly", 1);
session_start();
if(isset($_SESSION['username'])){
    unset($_SESSION['username']);
    session_destroy();
    echo json_encode(array(
        "success" => true
    ));
    exit;
}
?>
