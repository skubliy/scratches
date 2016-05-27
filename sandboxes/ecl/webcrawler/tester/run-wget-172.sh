#!/bin/bash

while :
do

#sleep 10

let pageId=$RANDOM%1000+1
let htmlSize=16
let cssCount=8
let cssSize=8
let cssKey=$RANDOM%256+1
let jsCount=8
let jsSize=16
let jsKey=$RANDOM%256+1
let imgCount=32
let imgSize=$RANDOM%12+1
let imgKey=$RANDOM%1028+1
let viewStateSize=$RANDOM%400+1
viewStateLocation="middle"
let viewStateKey=$RANDOM%10000+1
isChunked="true"

host="http://192.168.140.172"
port=":8888"
path="/SuperFlex.aspx?tag=$pageId&htmlSize=$htmlSize&cssCount=$cssCount&cssSize=$cssSize&cssKey=$cssKey&jsCount=$jsCount&jsSize=$jsSize&jsKey=$jsKey&imgCount=$imgCount&imgSize=$imgSize&imgKey=$imgKey&viewStateSize=$viewStateSize&viewStateLocation=$viewStateLocation&viewStateKey=$viewStateKey&isChunked=$isChunked"

response=$(wget -qp -nd --user-agent=\"MSIE\ 6.\" --delete-after $host$port$path)
echo -n "K7 "
#echo MSIE 7. Keynote >> output.txt
#echo $response >> output.txt
response=$(wget -qp -nd --user-agent=\"MSIE\ 7.\" --delete-after $host$port$path)
echo -n "M7 "

#echo MSIE 7 >> output.txt
#echo $response >> output.txt
response=$(wget -qp -nd --user-agent=\"Chrome/1\" --delete-after $host$port$path)
echo -n "C1 "
#echo Chrome 1 >> output.txt
#echo $response >> output.txt
response=$(wget -qp -nd --user-agent=\"AppleWebKit/53\" --delete-after $host$port$path)
echo -n "A5 "
#echo WebKit 53 >> output.txt
#echo $response >> output.txt
response=$(wget -qp -nd --user-agent=\"Trident/4.0\" --delete-after $host$port$path)
echo -n "M8 "
#echo Trident 4 >> output.txt
#echo $response >> output.txt
response=$(wget -qp -nd --user-agent=\"Trident/5.0\" --delete-after $host$port$path)
echo -n "M9 "
#echo Trident 5 >> output.txt
#echo $response >> output.txt
response=$(wget -qp -nd --user-agent=\"Mozilla/5.0\ \(Linux\;\ Android\ 4.2.2\;\ GT-I9505\ Build/JDQ39\)\ AppleWebKit/537.36\ \(KHTML,\ like\ Gecko\)\ Chrome/31.0.1650.59\ Mobile\ Safari/537.36\" --delete-after $host$port$path)
echo -n "MP "
#echo Trident 5 >> output.txt
#echo $response >> output.txt
response=$(wget -qp -nd --user-agent=\"Mozilla/5.0\ \(Linux\;\ Android\ 4.3\;\ Nexus\ 10\ Build/JSS15Q\)\ AppleWebKit/537.36\ \(KHTML,\ like\ Gecko\)\ Chrome/29.0.1547.72\ Safari/537.36\" --delete-after $host$port$path)
echo -n "MT "
#echo Trident 5 >> output.txt
#echo $response >> output.txt
date
done

