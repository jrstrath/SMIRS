<?php
    //include('../samples/connect.php');
    //units=For temperature in Celsius use units=metric
    //183595 is Ngong ID
    //184745 - Nairobi
    //API Key = 89650aff82771e8843cb773712a7e08e
 
    function forecast(){
        $url = "http://api.openweathermap.org/data/2.5/forecast?id=184745&lang=en&units=metric&APPID=89650aff82771e8843cb773712a7e08e";

        $contents = file_get_contents($url);
        $clima=json_decode($contents);
    
        foreach($clima->list as $value)
        {
            $temp = $value->main->temp;
            $humidity = $value->main->humidity;
            $rain = $value->weather[0]->description;
            $date = $value->dt_txt;

            //Send forecast data to db
            $query = mysqli_query($db, "INSERT INTO forecast(`date_time`, `temperature`, `humidity`, `rain`)
                                        VALUES ('$date', '$temp', '$humidity', '$rain')");
    
            if($query)
            {
                //Go to login page
            echo ("Data entry unsuccessful");
            }

            return "Forecast successful";
        }
    }
    
    function current_weather($item){
        $url = "http://api.openweathermap.org/data/2.5/weather?id=184745&lang=en&units=metric&APPID=89650aff82771e8843cb773712a7e08e";

        $contents = file_get_contents($url);
        $clima=json_decode($contents);

        $current_temp = $clima->main->temp;
        $current_humidity = $clima->main->humidity;
        $description = $clima->weather[0]->description;
        $icon = $clima->weather[0]->icon;

        if($item == 'temp'){
            return $current_temp;
        }
        elseif($item == 'humidity'){
            return $current_humidity;
        }
        elseif($item == 'description'){
            return $description;
        }
        elseif($item == 'icon'){
            $png = (string)$icon;
            $icon_url = "http://openweathermap.org/img/w/$png.png";
            return $icon_url;
        }
    
    }
?>

<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <title>Page Title</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" type="text/css" media="screen" href="main.css" />
    <script src="main.js"></script>
</head>
<body>
    
</body>
</html>