<?php session_start();
?>


<html>  

<head>
        <title>Login Form</title>
        <meta charset="UTF-8">
        <link href="_style.css" rel="stylesheet" type="text/css">
        <link href="https://fonts.googleapis.com/css?family=Spartan:600&display=swap" rel="stylesheet">
</head>

<body style="background-color:#FFFFFF">
         <!--have to change this to logout page that first destroys sessions etc..-->
         <div class="nav_button">
             <a href="signupform.php">Apply</a>
        </div>
        <div class="other">
            <a><?php echo "Welcome ".$firstname?></a>
        </div>


        <header>
            <p>OnePlace</P>
        </header> 
    
   <div class = "form">  
   <form method="post" action="home.php">
       <label for="username">Username</label><br>
        <input type="text" name="username" required/><br><br>
        <label for="password">Password</label><br>
        <input type="password" name="password" class ="psw" required/><br><br>
        <?php if(isset($_SESSION['error'])){echo "<p style='color:blue;font-size:11px;margin:unset;'>".$_SESSION['error']."</p><br>";}?>
        <input type="submit" class="button" name="submit" value="Login" /><br><br>
        <p class='no_account'> Don't have an account? Sign up <a href='signupform2.html'>here</a></p>
    </form>
    </div>
    

      <!--
    <footer class ="mod">
    
    </footer> -->
 </body>
</html>