<?php
	include('phpSerial.php');
    require('phpSerial.class');
    $serial = new phpSerial;
	$serial->deviceSet("/dev/ttyACM0");
	$serial->confBaudRate(9600);
	$serial->deviceOpen();
	$serial->sendMessage("get"); 
	$done = false;
	$line = "";
		while (!$done) 
		{
		$read = $serial->readPort();
			for ($i = 0; $i < strlen($read); $i++) 
			{
				if ($read[$i] == "\n") 
				{
				$done = true;
				echo 1; //$line;
				$line = "";
				} 
				else 
				{
				$line .= $read[$i];
				}
			}
		}
	$serial->deviceClose(); 
?>