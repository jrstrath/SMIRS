<?php
    define('DB_HOST', 'localhost');
    define('DB_NAME', 'smirs');
    define('DB_USER', 'root');
    define('DB_PASSWORD','');

    $con = mysql_connect(DB_HOST,DB_USER,DB_PASSWORD)
        or die("Failed to connect to MySQL: " . mysql_error());
    $db = mysql_select_db(DB_NAME, $con)
        or die("Failed to connect to MySQL: " . mysql_error());
    
    /*
        $ID = $_POST['username'];
        $Password = $_POST['pass'];
    */

    function SignIn(){
        session_start();    //Starting the session for the user profile
        if(!empty($_POST['username'])){
            $query = mysql_query("SELECT * FROM farmer 
                where username = '$_POST[Username]' 
                AND password = '$_POST[Password]'") 
            or die(mysql_error()); 
            
            $row = mysql_fetch_array($query) 
            or die(mysql_error());

            if(!empty($row['username']) AND !empty($row['password'])){
                $_SESSION['username'] = $row['password']; 
                echo "SUCCESSFULLY LOGIN"; 
            }
            else{
                echo "Failed Login... Please try again";
            }
        }
    }

    if(isset($_POST['submit'])){
        SignIn();
    }
?>