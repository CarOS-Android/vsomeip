# change workspace to the directory where you want to mount to docker by -v
colima start --cpu 2 --memory 8 --disk 30 --mount ~/workspace/:w --mount-type 9p 
