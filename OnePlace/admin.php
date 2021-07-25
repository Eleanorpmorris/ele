<?php
session_start();
$next
?>

<html>
<head></head>
<body>
    
    <p>Go back to<a href='index.php'> Login</a></p>
    <form method="POST" action="reports.php" >
        <p>Choose report type:</p>
        <input type='radio' id='search_vendor' name='choose' value="vendor_search">
        <label for='search_vendor'>Vendor reports</label>
          
        <br>
        <input type='radio' id='list_terms' name='choose' value="frequent_searches">
        <label for='list_terms'>Frequent search terms</label>
        <br><br>
        <input type='submit' name='submit' value='Submit'>
        

    </form>
</body>
</html>





