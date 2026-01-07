# Astonia 3.5 Server, Community Editon

## Warning
Do not deploy on an existing database! Table structure has changed and will
continue to change!

## Building

### Ubuntu

To run under Ubuntu 22.04.4 LTS (tested and working as of 2026.01.06)

```
sudo apt-get update
sudo apt install git
git clone https://github.com/DanielBrockhaus/astonia_server.git
sudo apt-get install gcc-multilib
sudo dpkg --add-architecture i386
sudo apt install make
sudo apt install lib32z-dev libargon2-dev:i386
sudo apt install libmysqlclient-dev:i386
sudo apt install mariadb-server
cat MYSQLPASSWD
sudo mysql_secure_installation
# old root password is blank. new root password from 'cat MYSQLPASSWD'.
# do not switch to unix sockets
./my <create_tables.sql 
./my merc <merc.sql 
echo "Welcome to Astonia" >motd.txt
rm MYSQLPASSWD
# the "my" script is very handy, but also a security risk!
```
