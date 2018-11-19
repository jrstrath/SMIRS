<?php
    include ("connect.php");

    $humidity = $_POST['humidity'];
    $temperature = $_POST['temperature'];
    $soil_moisture = $_POST['soilmoisture'];
    $flow_rate = $_POST['flowrate'];
    $volume = $_POST['volume'];

    $query = mysqli_query($db, "INSERT INTO parameters(`temperature`, `humidity`, `soil_moisture`, `flow_rate`, `volume`)
                                        VALUES ('$temperature', '$humidity', '$soil_moisture', '$flow_rate', '$volume')");
?>