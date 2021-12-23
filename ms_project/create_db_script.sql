CREATE TABLE city
(
  id serial UNIQUE NOT NULL,
  city_name character varying NOT NULL,
  CONSTRAINT pk_city_date PRIMARY KEY (id)
);

CREATE TABLE dayweek
(
  id serial UNIQUE NOT NULL,
  dayweek character varying NOT NULL,
  CONSTRAINT pk_dayweek PRIMARY KEY (id)
);

CREATE TABLE floor
(
  id serial UNIQUE NOT NULL,
  floor integer NOT NULL,
  CONSTRAINT pk_floor PRIMARY KEY (id)
);

CREATE TABLE guest
(
  first_name character varying NOT NULL,
  middle_name character varying NOT NULL,
  last_name character varying NOT NULL,
  passport UNIQUE NOT NULL,
  city_id integer,
  CONSTRAINT pk_guest PRIMARY KEY (passport),
  CONSTRAINT guest_city_id_fkey FOREIGN KEY (city_id)
      REFERENCES city (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
);

CREATE TABLE room_type
(
  id serial UNIQUE NOT NULL,
  capacity integer NOT NULL,
  price integer NOT NULL,
  CONSTRAINT pk_room_type PRIMARY KEY (id)
);

CREATE TABLE room
(
  id serial UNIQUE NOT NULL,
  phone integer NOT NULL,
  floor_id integer NOT NULL,
  room_type_id integer NOT NULL,
  is_available boolean NOT NULL,
  CONSTRAINT pk_room PRIMARY KEY (id),
  CONSTRAINT room_floor_id_fkey FOREIGN KEY (floor_id)
      REFERENCES floor (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT room_room_type_id_fkey FOREIGN KEY (room_type_id)
      REFERENCES room_type (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
);

CREATE TABLE worker
(
  id serial UNIQUE NOT NULL,
  first_name character varying NOT NULL,
  middle_name character varying NOT NULL,
  last_name character varying NOT NULL,
  CONSTRAINT pk_worker PRIMARY KEY (id)
);

CREATE TABLE contract
(
  id serial UNIQUE NOT NULL,
  arrival date NOT NULL,
  duration integer NOT NULL,
  room_id integer NOT NULL,
  guest_id numeric NOT NULL,
  CONSTRAINT pk_contract PRIMARY KEY (id),
  CONSTRAINT contract_guest_id_fkey FOREIGN KEY (guest_id)
      REFERENCES guest (passport) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT contract_room_id_fkey FOREIGN KEY (room_id)
      REFERENCES room (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
);

CREATE TABLE current_guest_list
(
  id serial UNIQUE NOT NULL,
  guest_id numeric NOT NULL,
  companion_1_id numeric,
  companion_2_id numeric,
  room_id integer,
  CONSTRAINT pk_residents_list PRIMARY KEY (id),
  CONSTRAINT residents_list_companion_1_id_fkey FOREIGN KEY (companion_1_id)
      REFERENCES guest (passport) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT residents_list_companion_2_id_fkey FOREIGN KEY (companion_2_id)
      REFERENCES guest (passport) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT residents_list_guest_id_fkey FOREIGN KEY (guest_id)
      REFERENCES guest (passport) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT residents_list_room_id_fkey FOREIGN KEY (room_id)
      REFERENCES room (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT current_guest_list_guest_id_key UNIQUE (guest_id),
  CONSTRAINT current_guest_list_room_id_key UNIQUE (room_id)
);

CREATE TABLE cleaning_schedule
(
  id serial UNIQUE NOT NULL,
  floor_id integer NOT NULL,
  dayweek_id integer NOT NULL,
  worker_id integer,
  CONSTRAINT pk_cleaning_schedule PRIMARY KEY (id),
  CONSTRAINT cleaning_schedule_dayweek_id_fkey FOREIGN KEY (dayweek_id)
      REFERENCES dayweek (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT cleaning_schedule_worker_id_fkey FOREIGN KEY (worker_id)
      REFERENCES worker (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
);

CREATE USER admin WITH password 'admin';
GRANT SELECT, UPDATE, INSERT, DELETE ON city TO admin;
GRANT SELECT, UPDATE, INSERT, DELETE ON cleaning_schedule TO admin;
GRANT SELECT, UPDATE, INSERT, DELETE ON contract TO admin;
GRANT SELECT, UPDATE, INSERT, DELETE ON contract TO admin;
GRANT SELECT, UPDATE, INSERT, DELETE ON dayweek TO admin;
GRANT SELECT, UPDATE, INSERT, DELETE ON floor TO admin;
GRANT SELECT, UPDATE, INSERT, DELETE ON guest TO admin;
GRANT SELECT, UPDATE, INSERT, DELETE ON room TO admin;
GRANT SELECT, UPDATE, INSERT, DELETE ON room_type TO admin;
GRANT SELECT, UPDATE, INSERT, DELETE ON worker TO admin;

CREATE USER director WITH password 'director';
GRANT SELECT, UPDATE ON cleaning_schedule TO manager;
GRANT SELECT ON dayweek TO manager;
GRANT SELECT ON floor TO manager;
GRANT SELECT, UPDATE, INSERT, DELETE ON worker TO manager;

CREATE USER manager WITH password ' manager ';
GRANT SELECT, INSERT ON city TO manager;
GRANT SELECT ON cleaning_schedule TO manager;
GRANT SELECT, INSERT, DELETE ON current_guest_list TO manager;
GRANT SELECT, INSERT ON contract TO manager;
GRANT SELECT ON dayweek TO manager;
GRANT SELECT ON floor TO manager;
GRANT SELECT, INSERT ON guest TO manager;
GRANT SELECT, UPDATE ON room TO manager;
GRANT SELECT ON room_type TO manager;
GRANT SELECT ON worker TO manager;
