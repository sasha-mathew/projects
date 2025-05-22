To Run Assignment:
1. Create VM instance
2. upload files to SSH
3. mkdir project
4. mv ~/docker-compose.yml ~/hadoop.env ~/nbs ~/project/
5. sudo chmod -R 777 nbs/
   docker-compose up -d
6. open Jupyter Notebook through http://external_ip:8888
7. docker ps
   docker exec <container_id> hdfs dfs -put /home/nbs/goalscorers.txt /goalscorers.txt
   docker exec <container_id> hdfs dfs -put /home/nbs/results.txt /results.txt
   docker exec <container_id> hdfs dfs -put /home/nbs/shootouts.txt /shootouts.txt
where container_id is namenode container id
8. can see files in Hadoop through http://external_ip/explorer.html

