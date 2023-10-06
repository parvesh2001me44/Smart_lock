<?php
// Retrieve user input from form
$username = $_POST['username'];
$password = $_POST['password'];
$email = $_POST['email'];

// Hash the password for secure storage
$hashed_password = password_hash($password, PASSWORD_DEFAULT);

// Connect to MySQL database
$servername = "localhost";
$db_username = "haryana";
$db_password = "Abhi@93405";
$dbname = "DB";

$conn = new mysqli($servername, $db_username, $db_password, $dbname);

// Check if connection was successful
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}

// Create table to store user information if it doesn't exist
$sql = "CREATE TABLE IF NOT EXISTS accounts (
        id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY,
        username VARCHAR(30) NOT NULL,
        password VARCHAR(255) NOT NULL,
        email VARCHAR(50),
        reg_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
      )";

if ($conn->query($sql) === FALSE) {
  echo "Error creating table: " . $conn->error;
}

// Insert user information into table
$sql = "INSERT INTO accounts (username, password, email) VALUES ('$username', '$hashed_password', '$email')";

if ($conn->query($sql) === TRUE) {
  echo "New record created successfully";
} else {
  echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();

// Redirect to authenticate.php
header("Location: login.html");
exit();
?>
