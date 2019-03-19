<!-- refrence https://classes.engineering.wustl.edu/cse330/index.php?title=Module_3 -->
<?php
header("Content-Type: application/json");
require 'database.php';
// we have to request the database in order to get the data from it
ini_set("session.cookie_httponly", 1);
session_start();
//using session to get user info
if (isset($_SESSION['username'])) {
    $username = $_SESSION['username'];
// query the table to get info
    $stmt = $mysqli->prepare("select time from feed where username=?");
    if(!$stmt){
        printf("Query Prep Failed: %s\n", $mysqli->error);
        echo json_encode(array(
            "success" => false,
            "message" => "Query prep failed."
        ));
        exit;
    }
    $stmt->bind_param('s', $username);
    $stmt->execute();
    $stmt->bind_result($time);
    $array_time = array();
    $array_tag = array();
    while($stmt->fetch()) {
// take the time and push it to font-end 
        array_push($array_time, $time);
    }
    $stmt->close();
    echo json_encode(array(
        "success" => true,
        "message" => "showing",
        "time" => $array_time,
    ));
    exit;
}
else{
    echo json_encode(array(
        "success" => false,
        "message" => "You are not a user."
    ));
}
?>
