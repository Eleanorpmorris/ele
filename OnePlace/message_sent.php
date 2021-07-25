<?php
session_start();


include 'functions.php';
include 'class.DB.php';

$firstname = $_SESSION['vendor'];
$msgTitle = $_POST['title'];
$msgContent = $_POST['message'];
$vendor = $_SESSION['user'];

$date = getdate();
$datadate = $date['year']."-".$date['mon']."-".$date['mday'];

$cust_list = implode(", ", $_SESSION['users']);
print_r($cust_list);
$customers = explode(" ", $_SESSION['users']);
foreach ($customers as $cust){
$sql = "INSERT into messages VALUES('".$cust."','".$msgTitle."','".$msgContent."','".$vendor."','".$datadate."')";
$result = DB::query_insert($sql);
}

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
            <!--have to change this to logout page that first destroys sessions etc..-->
            <a href="logout.php">Logout</a>
        </div>
        <div class="nav_button">
            <a href="home.php?<?php echo $_SESSION['user']?>">Search</a>
        </div>
        <div class="other">
            <a><?php echo "Welcome<br>".$firstname?></a>
        </div>


        <header>
            <p>OnePlace</P>
        </header>
        
        
        <?php  if ($result === TRUE){
        ?>
        <div class='message_sent'>
        <p>Thanks <?php echo $firstname ?></p>
        <p>The following message has been sent to customer numbers: <?php echo implode(' ', $customers); ?>
        
         </p><br>
         <table>
             <tr><th>Title:  <?php echo $msgTitle ?> </th></tr>       
             <tr><th class='content'>Content:  <?php echo $msgContent ?> </th></tr> 
        </table
        </div>
        <?php      
        }else{
            echo "Sorry, something went wrong.";
        }
        ?>
        
    </body>
</html>
