<?php
session_start();


$vendor = $_SESSION['user'];
$firstname = $_SESSION['vendor'];

$users = $_POST['users'];
$users = implode(", ", $users);
$_SESSION['users'] = $users;


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
         <!-- <div class="nav_button">
            <a href="search.php">Results</a>
        </div>-->

        <div class="nav_button">
            <a href="home.php?<?php echo $_SESSION['user']?>">Search</a>
        </div>
        <div class="other">
            <a><?php echo "Welcome ".$firstname;?></a>
        </div>


        <header>
            <p>OnePlace</P>
        </header>
<form action="message_sent.php?<?php echo $_SESSION[users]?>" method="POST">
    
    <p>Your Message will be sent to the following customers: <?php echo $users?></p><br>
    <input type="text" name="title" placeholder="Enter the title of your messege"><br>
    <textarea class="message" name="message" placeholder="Write your message..." style="width:30%; height: 200px;"></textarea><br>
    <input type="submit" value="Send Message">
    
</form>
    </body>
</html>