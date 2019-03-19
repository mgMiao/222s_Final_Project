<?php
	// addDate.php
header("Content-Type: application/json");
// Since we are sending a JSON response here (not an HTML document), set the MIME Type to application/json
require 'database.php';
$json_str = file_get_contents('php://input');
$json_obj = json_decode($json_str, true);
$time = $json_obj['time'];
ini_set("session.cookie_httponly", 1);
session_start();
if (isset($_SESSION['username'])){
    $username = $_SESSION['username'];

    if ($time == "") {
        echo json_encode(array(
		    "success" => false,
		    "message" => "Your inputs may be empty. Please type a valid value."
	    ));
	    exit;
    }
// query our database table to store the information
    $stmt = $mysqli->prepare("insert into feed ( time, username) values (?, ?)");
    if(!$stmt){
        printf("Query Prep Failed here: %s\n", $mysqli->error);
        exit;
    }
    $stmt->bind_param('ss', $time, $username);
    $stmt->execute();
    $stmt->close();
    echo json_encode(array(
        "success" => true,
        "message" => "Your time is recorded!"
    ));
	exit;
} else {
    echo json_encode(array(
		"success" => false,
        "message" => "You are not a user, Please sign in!"
	));
	exit;
}
?>
