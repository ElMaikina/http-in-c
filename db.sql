drop database if exists BackEnd_in_C;
create database if not exists BackEnd_in_C;
use BackEnd_in_C;

drop table if exists `users`;
create table `users` (
  `id` bigint not null auto_increment,
  `email` varchar(100) default null,
  `password` varchar(50) default null,
  primary key (`id`)
);

drop table if exists `tasks`;
create table `tasks` (
  `id` bigint not null auto_increment,
  `details` varchar(100) default null,
  `user_id` bigint not null,
  foreign key (user_id) references users(id),
  primary key (`id`)
);