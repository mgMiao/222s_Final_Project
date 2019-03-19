<?php
	// signup_ajax.php
header("Content-Type: application/json"); // Since we are sending a JSON response here (not an HTML document), set the MIME Type to application/json
require 'database.php';
//Because you are posting the data via fetch(), php has to retrieve it elsewhere.
$json_str = file_get_contents('php://input');
//This will store the data into an associative array
$json_obj = json_decode($json_str, true);

//Variables can be accessed as such:
$username = $json_obj['username'];
$password = $json_obj['password'];
$pwd_hash = password_hash($password, PASSWORD_BCRYPT);

//We first check to see if the username has already been registered
$stmt = $mysqli->prepare("select username from users");
	if(!$stmt){
	printf("Query Prep Failed: %s\n", $mysqli->error);
	exit;
	}
	$stmt->execute();
	$stmt->bind_result($users);
	while($stmt->fetch()){
		if($username==$users){
			echo json_encode(array(
				"success" => false,
				"message" => "User Already Exists"
			));
			$stmt->close();
			exit;
		 	}
	}
//Then we the store the new user into our database
$stmt = $mysqli -> prepare("insert into users (username, password) values (?, ?)");
	if(!$stmt){
		printf("Query Prep Failed here: %s\n", $mysqli->error);
		exit;
	}
	$stmt->bind_param('ss', $username, $pwd_hash);
	$stmt->execute();
	ini_set("session.cookie_httponly", 1);
	session_start();
	$_SESSION['username'] = $username;
	$_SESSION['token'] = bin2hex(openssl_random_pseudo_bytes(32));

	echo json_encode(array(
		"success" => true,
		"message" => "You 've been logged in",
		"username" => $username
	));
	$stmt->close();
	exit;
?>