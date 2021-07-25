<?php
session_start();

$firstname = $_SESSION['vendor'];

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
            <a><?php echo "Welcome ".$firstname?></a>
        </div>


        <header>
            <p>OnePlace</P>
        </header>
         <div class ="colmask">
           
<?php
include 'functions.php';
include 'class.DB.php';

$term = $search_results = " ";

//only proceed if the user has entered something in the search bar
if (isset($_POST['search'])) {
    $term = $_POST['keywords'];
} else {
    echo "not getting keywords<br>";
}
//$keywords = filterSearchKeys($term);
//needed for highlighting only the relevant search terms(not stopwords)
$terms_to_highlight = implode(" ", filterSearchKeys($term));

//perform the search
$search_results = search($term);

//display the search results
foreach ($search_results as $result) {
    $result_count ++;
    ?>
    <div class="data">
        
    <?php
     echo "<br><br>";
    foreach ($result as $key => $value) {
       
        if ($key === 'usrID'){
            $usrID = $value;
            echo "<hr>";
            echo "<p class='title'>User: ".$usrID."<br></p>";
        }
        if ($key === 'infoToShare') {
            echo "<p class='infoToShare'><br>";
            echo highlight_keywords($value, $terms_to_highlight) . "<br>";  //this is highlighting all the opriginal words becuas of $term.  how to only highlight ofter filtered.
            echo "</p>";
        }
        if ($key === 'title') {
            echo "<p class='title'>";
            echo "Title: ".highlight_keywords($value, $terms_to_highlight) . "<br>";
            echo "</p>";
        }
        if ($key === 'dateShared'){
            echo "<p class='date'>";
            echo "First shared on: ".$value."<br>";
            echo "</p>";
        }
        if ($key === 'relevance'){
            echo "<p class='relevance'>";
            echo "(Relevance score: ".$value.")<br>";
            echo "</p>";
        }
    
    }
        echo "<form method='POST' action='send_message.php?<?php echo $_SESSION[user]?>'>";
        echo "<input class='checkbox' type='checkbox' name='users[]' value='".$usrID."'><label for='select'>Add to message.</label>";       
}

 
if (!$search_results) {
    echo 'No results for that...try searching for something else. <a href="home.php">Back</a>';
    exit;
}else{
    echo "<div class='rt'>Check each user you would like to add to this message and hit Select.";
    echo "<input class='rt select' type='submit' value='Select'></div>";
    echo "</form>";
}

?>
    </div>                        
       </div>
       <?php echo "<p class='result_count'>Your search for '".$term."' returned --".$result_count."-- results:</p><br>";
       ?>
        
    
    </body>
    
    <footer>
        Thats all we found for now...<br><br>
        <hr class="end_hr">
        Created by Team Nigel
    </footer>
</html>

