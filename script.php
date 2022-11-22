<?php
$time = date("Y-m-d H:i:s",strtotime('-1 hours'));
$dbname = "esp_data";
$dbuser = "admin";
$dbpass = "password";
$dbhost = "localhost";

$connect = mysqli_connect($dbhost,$dbuser,$dbpass,$dbname);
if (!$connect){
echo "error:";
}
echo "connection succesful.";
$s = $_POST["src"];
$t = $_POST["temp"];
$h = $_POST["hum"];
$q = $_POST["qa"];

$s1 = explode("j",$s);
$t1 = explode("j",$t);
$h1 = explode("j",$h);
$q1 = explode("j",$q);
$a = $s1[0];
$b = $t1[0];
$c = $h1[0];
$q = $q1[0];
$query = "INSERT INTO esp$a (reading_time,temperature,humidity,qualite_air) VALUES ('$time','$b','$c','$q');";
mysqli_query($connect,$query);

$a = $s1[1];
$b = $t1[1];
$c = $h1[1];
$q = $q1[1];
$query = "INSERT INTO esp$a (reading_time,temperature,humidity,qualite_air) VALUES ('$time','$b','$c','$q');";
mysqli_query($connect,$query);

?>