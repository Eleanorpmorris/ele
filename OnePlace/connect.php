<?php
class DB {

    private static $host = "69.172.204.200";
    private static $user = "eleamormorris";
    private static $pass = "Salmonarm5532";
    private static $dbname = "eleamorm_data";
    private static $con;

    private static  function connect(){
        if (self::$con){
            return self::$con;
        }
        self::$con = new mysqli(self::$host,self::$user,self::$pass,self::$dbname);

        if (!self::$con){
            echo "There seems to be a problem with your connection.";
            exit;
        }

        return self::$con;
    }


    public static function query($sql){
        $con = self::connect();

        $result = $con->query($sql);

        if (!$result){
            // Then there is probably an error in the query
            // Add a nice error handler here
            echo "Error.  \nIn this query:\"<i>$sql</i>\"\n$con->error";
            exit;
        }
        if (isset($result->field_count) && $result->field_count == 0){
            return false;
        }

        $group = array();
        while (($r = $result->fetch_object()) != false){
            $group[] = $r;
        }
        return $group;
    }

    public static function escape($string){
        $con = self::connect();
        return $con->real_escape_string($string);
    }
}

