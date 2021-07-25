<?php

session_start();


session_unset();
session_destroy();
session_write_close();
session_regenerate_id(true);
if (isset($_COOKIE[session_name()])) {
    setcookie(session_name(), '', time()-42000, '/');
}



 
?>
        <html><body>
                <script> window.location.replace('index.php');
                </script>
                    
    </body></html>