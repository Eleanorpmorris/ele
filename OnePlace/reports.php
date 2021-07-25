<?php
session_start();

if(isset($_POST['submit'])){
    
    $report_type = filter_input(INPUT_POST, 'choose');
    echo "Enter the Vendor details below to get the associated reports.<br><br>";
}else{
     echo "<script> window.location.replace('admin.php') </script>"; 
}
if($report_type === 'frequent_searches'){
?>
<form method='POST' action='term_search.php'>
    <input type='text' name='terms'><label for='terms'>Search for which term?</label>
    <input type='submit' value='Submit' name='submit'>
</form>
<?php
}
if($report_type === 'vendor_search'){
?>
<form method='POST' action='vendor_reports.php'>
    <label for='f_vendor'>First name</label><br><input type='text' name='f_vendor'><br>
    <label for='l_vendor'>Last name</label><br><input type='text' name='l_vendor'><br><br>
    <input type='submit' value='Submit' name='submit'>
</form>
<?php
}



