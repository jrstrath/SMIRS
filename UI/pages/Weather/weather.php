<?php
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
    
    function current_weather($id,$item){
        $url = "http://api.openweathermap.org/data/2.5/weather?id=$id&lang=en&units=metric&APPID=89650aff82771e8843cb773712a7e08e";

        $contents = file_get_contents($url);
        $clima=json_decode($contents);
        
        
        $current_temp = $clima->main->temp;
        $current_humidity = $clima->main->humidity;
        $description = $clima->weather[0]->description;
        $icon = $clima->weather[0]->icon;
        $country = $clima->sys->country;
        $town = $clima->name;
        
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
        elseif($item == 'country'){
            return $country;
        }
        elseif($item == 'town'){
            return $town;
        }
    }

    function mapping(){
        $file = "citylist.json";

        $contents = file_get_contents($file);
        $city=json_decode($contents);

        foreach($city as $value){
            $id = $value->id;
            $name = $value->name;

            //Send forecast data to db
            $query = mysqli_query($db, "INSERT INTO city_mapping(`city_id`, `city_name`)
                                        VALUES ('$id', '$name')");

        }
    }
?>
