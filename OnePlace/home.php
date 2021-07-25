<?php
session_start();

$SID = session_id();
//connect
$mysqli = mysqli_connect("69.172.204.200", "eleamormorris", "Salmonarm5532", "eleamorm_data");
// Check connection
if($mysqli === false){
    die("ERROR: Could not connect. " . mysqli_connect_error());
}
$username = $password = $firstname = $error ="";



//veriables for the user email and password provided
$username = filter_input(INPUT_POST, 'username');
$password = filter_input(INPUT_POST, 'password');
$firstname;
$lastname;
$username = stripslashes($username);
$password = stripslashes($password);
$username = mysqli_escape_string($mysqli, $username);
$password = mysqli_escape_string($mysqli, $password);

//if admin is signing in redirect to admin pages
if($username === 'admin' && $password === 'admin'){echo "<script> window.location.replace('admin.php') </script>";} 

//set up the variable that contains the sql query for checking the email and password
$sql = "SELECT username, password, f_name, l_name FROM users WHERE username = '".$username.
        "' AND password = PASSWORD('".$password."')";

//perform a quesry and store the result in a variable
$result = mysqli_query($mysqli, $sql) or die(mysqli_error($mysqli));

$rows = mysqli_num_rows($result);

if ($rows === 1){ session_start(); $_SESSION['user']=$username;} // Initializing Session

if (isset($_SESSION['user'])){
   
  	//get the values of f_name l_name etc.
	while ($info = mysqli_fetch_array($result)) {
		$user = stripslashes($info['username']);
                $firstname = stripslashes($info['f_name']);
                $lastname = stripslashes($info['l_name']);
                $password = stripslashes($info['password']);
        }
       
        $_SESSION['vendor'] = $firstname;        
?>

<html>
    <head>
        <meta charset="UTF-8">
        <link href="_style.css" rel="stylesheet" type="text/css">
        <link href="https://fonts.googleapis.com/css?family=Spartan:600&display=swap" rel="stylesheet">
        <title>Oneplace Vendor Marketplace</title>
    </head>
    <body>

        <div class="nav_button">
            <a href="logout.php">Logout</a>
        </div>
        
        <div class="nav_button">
            <a href="home.php?<?php echo $firstname ?>">Search</a>
        </div>
        
        <div class="other">
            <a><?php echo "Welcome ".$firstname?></a>
        </div>

        <header>
            <p>OnePlace</P>
        </header>
        <div class ="colmask">
            <div class ="col1">
                <form method ="POST" action ="search.php">
                    <input type ="text" class="text" name ="keywords" placeholder="Search by phrase">
                    <input type ="text" class="text" name ="location" placeholder="Search by location"><br>
                    <input type="submit" class="button" name ="search" value ="Search">
                </form>
            </div>
        </div>
    

<?php
} else{
    
    $_SESSION['error'] = "*Something is wrong with your username or password.  Try something else";
    echo "redirecting...";?>

    <script>
        window.location.replace('index.php');
    </script>
    
 <?php  
}
?>
</body>
</html>