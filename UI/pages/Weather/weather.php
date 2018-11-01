<?php
    //units=For temperature in Celsius use units=metric
    //183595 is Ngong ID
    //184745 - Nairobi
    //API Key = 89650aff82771e8843cb773712a7e08e

    $url = "http://api.openweathermap.org/data/2.5/forecast?id=184745&lang=en&units=metric&APPID=89650aff82771e8843cb773712a7e08e";


    $contents = file_get_contents($url);
    $clima=json_decode($contents, true);

    //$temp=$clima->main->temp;
    //$temp_min=$clima->main->temp_min;
    //$icon=$clima->weather[0]->icon.".png";
    
    //how get today date time PHP :P
    //$today = date("F j, Y, g:i a");
    //$cityname = $clima->name; 
    
    //print_r($clima);
    foreach($clima as $key=>$value){
        echo $key . "=>" . $value . "<br>";
    }

    //$temp = $clima['list']['dt']  ;
    //print_r($temp);

    //echo $cityname . " - " .$today . "<br>";
    //echo "Temp: " . $temp ."&deg;C<br>";
    //echo "<img src='http://openweathermap.org/img/w/" . $icon ."'/ >";

?>