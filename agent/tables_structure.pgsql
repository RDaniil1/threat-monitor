CREATE DATABASE monitor;

--Action patterns
CREATE TABLE action_patterns (id INT NOT NULL PRIMARY KEY, name VARCHAR NOT NULL, pattern VARCHAR NOT NULL);

INSERT INTO action_patterns (id, name, pattern) VALUES (0, 'No action', 'No action');
INSERT INTO action_patterns (id, name, pattern) VALUES (1, 'Superuser action', 'session opened for user root');
INSERT INTO action_patterns (id, name, pattern) VALUES (2, 'User is not in sudoers group', 'user NOT in sudoers');
INSERT INTO action_patterns (id, name, pattern) VALUES (3, 'USB device connected', 'new USB device found');
INSERT INTO action_patterns (id, name, pattern) VALUES (4, 'Actions with shadow file', 'shadow_actions');
INSERT INTO action_patterns (id, name, pattern) VALUES (5, 'Firefox usage', 'firefox_exec');
INSERT INTO action_patterns (id, name, pattern) VALUES (6, 'Attempt to install program using apt', 'program_install_apt');
INSERT INTO action_patterns (id, name, pattern) VALUES (7, 'Attempt to install program using dpkg', 'program_install_dpkg');

--Employee
CREATE TABLE employee (id serial not null primary key, username varchar not null, machine_id varchar not null);

--Employee data
CREATE TABLE employee_data (id serial not null primary key, user_id serial not null, actions int[] not null, network_usage float, foreign key (user_id) references employee(id) on delete cascade);

--Actions predictions
CREATE TABLE actions_predictions (id serial not null primary key, user_id serial not null, violations_amount int not null, predicted_action serial not null, 
foreign key (user_id) references employee(id) on delete cascade, 
foreign key (predicted_action) references action_patterns(id) on delete cascade);

--Network predictions
CREATE TABLE network_predictions (id SERIAL NOT NULL PRIMARY KEY, prediction FLOAT[] NOT NULL);

CREATE ROLE regular_role WITH login;
GRANT SELECT, INSERT ON employee TO regular_role;
GRANT INSERT ON employee_data TO regular_role;
GRANT SELECT ON action_patterns TO regular_role;
GRANT USAGE, SELECT ON SEQUENCE employee_data_id_seq, employee_data_user_id_seq, employee_id_seq TO regular_role;

CREATE ROLE server_role WITH login;
GRANT SELECT ON employee_data TO server_role;
GRANT INSERT ON network_predictions TO server_role;
GRANT SELECT, INSERT, UPDATE ON actions_predictions TO server_role;
GRANT SELECT ON employee TO server_role;
GRANT USAGE, SELECT ON SEQUENCE employee_data_id_seq, employee_id_seq, actions_predictions_id_seq, network_predictions_id_seq TO server_role;

CREATE ROLE observer_role WITH login;
GRANT SELECT ON actions_predictions, employee_data, network_predictions, action_patterns, employee TO observer_role;

create user _server password 'server';
create user _user password 'user';
create user _observer password 'observer';

grant server_role to _server;
grant regular_role to _user;
grant observer_role to _observer;

CREATE OR REPLACE FUNCTION gathered_network(_row_limit float)
  RETURNS table (current_network_load float)
  LANGUAGE plpgsql AS
$$
DECLARE
   _employee_amount integer := (SELECT COUNT(DISTINCT user_id) FROM employee_data);
   _max_offset integer := (_row_limit - 1) * _employee_amount; 
   _offset integer := 0;
BEGIN
LOOP
   RETURN QUERY SELECT SUM(network_usage) FROM (
         SELECT * FROM employee_data ORDER BY id DESC LIMIT _employee_amount OFFSET _offset
         ) 
    AS sorted_employee_data;

   EXIT WHEN NOT FOUND OR _offset = _max_offset;
   _offset := _offset + _employee_amount;
END LOOP;
END
$$;