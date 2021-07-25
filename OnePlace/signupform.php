<?php 
session_start();
$_SESSION['error'] = "";
?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
        <link href="_style.css" rel="stylesheet" type="text/css">
        <link href="https://fonts.googleapis.com/css?family=Spartan:600&display=swap" rel="stylesheet">

</head>

<body style="background-color:#FEF9F2;"> 
    
            <div class="nav_button">
            <a href="index.php">Login</a>
        </div>
       <!-- <div class="other">
            <a>Welcome</a>
        </div>-->
    <header>

        <h1>  <p>OnePlace</P></h1>
        </header>
    
    
<form action="signup.php" method ="post">
  
    <h3> Enter your details below </h3><br>
    <h3>
    First name:<br>
    <input type="text" name="firstname"  pattern="^[a-zA-Z\s]*$" required> 
    <br><br>
    Last name:<br>
     <input type="text" name="lastname"   pattern="^[a-zA-Z\s]*$" required>
    <br><br>  
    Email:<br>
     <input type = "email" name = "email" required>
    <br> <br>
    Password:<br>
    <input type="password" name="password"  pattern="(?=.*\d)(?=.*[a-z])(?=.*[A-Z]).{8,}" title="Must contain at least one number and one uppercase and lowercase letter, and at least 8 or more characters" required/><br><br>
    Enter your password again:<br>
    <input type="password" name="password2" pattern="(?=.*\d)(?=.*[a-z])(?=.*[A-Z]).{8,}" required/><br><br>
    City:<br>
    <input type="text" name="city" pattern="^[a-zA-Z\s]*$" required /><br><br>
    Postal Code:<br>
    <input type="text" name="postal" placeholder="i.e. V1E2Y7" pattern="^[ABCEGHJKLMNPRSTVXYabceghjklmnprstvxy]{1}\d{1}[A-Za-z]{1}\d{1}[A-Za-z]{1}\d{1}$" required><br><br>
 
    Subject:<br>
    <textarea id="subject" name="subject" placeholder="Tell us more about yourself." style="height:200px; width:30%" ></textarea><br><br>  
    <input type="submit" name="submit" value="Create Account">
    
    </h3>
    </form>
    
</body>
</html>