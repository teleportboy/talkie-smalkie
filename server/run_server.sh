./server &
  
hasses -p=8080 -murl=/sseevents -fifo=/var/run/hassesfifo -nodaemon &

wait -n  

exit $?