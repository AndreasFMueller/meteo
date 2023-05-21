--
-- powermeter.sql
--
-- (c) 2023 Prof Dr Andreas Müller
--
insert into station(name, id, timezone, offset, longitude, latitude, altitude)
	values('Solivia', 6, 'MET', 3600, 47.18, 8.81, 610);

insert into sensor (name, id, stationid) values ('phase1', 12, 6);
insert into sensor (name, id, stationid) values ('phase2', 13, 6);
insert into sensor (name, id, stationid) values ('phase3', 14, 6);
insert into sensor (name, id, stationid) values ('string1', 15, 6);
insert into sensor (name, id, stationid) values ('string2', 16, 6);
insert into sensor (name, id, stationid) values ('inverter', 17, 6);

insert into mfield(name, id, unit, class)
	values('voltage', 40, 'V', 'VoltageValue');
insert into mfield(name, id, unit, class)
	values('voltage_min', 41, 'V', 'VoltageValue');
insert into mfield(name, id, unit, class)
	values('voltage_max', 42, 'V', 'VoltageValue');

insert into mfield(name, id, unit, class)
	values('current', 43, 'A', 'CurrentValue');
insert into mfield(name, id, unit, class)
	values('current_min', 44, 'A', 'CurrentValue');
insert into mfield(name, id, unit, class)
	values('current_max', 45, 'A', 'CurrentValue');

insert into mfield(name, id, unit, class)
	values('power', 46, 'W', 'PowerValue');
insert into mfield(name, id, unit, class)
	values('power_min', 47, 'W', 'PowerValue');
insert into mfield(name, id, unit, class)
	values('power_max', 48, 'W', 'PowerValue');

insert into mfield(name, id, unit)
	values('frequency', 49, 'Hz');

insert into mfield(name, id, unit)
	values('feedtime', 50, 's');
insert into mfield(name, id, unit, class)
	values('energy', 51, 'J', 'EnergyValue');
insert into mfield(name, id, unit, class)
	values('temperature', 52, 'C', 'TemperatureValue');
insert into mfield(name, id, unit, class)
	values('temperature_min', 53, 'C', 'TemperatureValue');
insert into mfield(name, id, unit, class)
	values('temperature_max', 54, 'C', 'TemperatureValue');

--
-- configuration of delta solivia 10 tl
--
-- 
-- Solivia.phase1.voltage              6.12.40
-- Solivia.phase1.voltage_min          6.12.41
-- Solivia.phase1.voltage_max          6.12.42
-- Solivia.phase1.current              6.12.43
-- Solivia.phase1.current_min          6.12.44
-- Solivia.phase1.current_max          6.12.45
-- Solivia.phase1.power                6.12.46
-- Solivia.phase1.power_min            6.12.47
-- Solivia.phase1.power_max            6.12.48
-- Solivia.phase1.frequency            6.12.49
-- 
-- Solivia.phase2.voltage              6.13.40
-- Solivia.phase2.voltage_min          6.13.41
-- Solivia.phase2.voltage_max          6.13.42
-- Solivia.phase2.current              6.13.43
-- Solivia.phase2.current_min          6.13.44
-- Solivia.phase2.current_max          6.13.45
-- Solivia.phase2.power                6.13.46
-- Solivia.phase2.power_min            6.13.47
-- Solivia.phase2.power_max            6.13.48
-- Solivia.phase2.frequency            6.13.49
-- 
-- Solivia.phase3.voltage              6.14.40
-- Solivia.phase3.voltage_min          6.14.41
-- Solivia.phase3.voltage_max          6.14.42
-- Solivia.phase3.current              6.14.43
-- Solivia.phase3.current_min          6.14.44
-- Solivia.phase3.current_max          6.14.45
-- Solivia.phase3.power                6.14.46
-- Solivia.phase3.power_min            6.14.47
-- Solivia.phase3.power_max            6.14.48
-- Solivia.phase2.frequency            6.14.49
-- 
-- Solivia.string1.voltage             6.15.40
-- Solivia.string1.voltage_min         6.15.41
-- Solivia.string1.voltage_max         6.15.42
-- Solivia.string1.current             6.15.43
-- Solivia.string1.current_min         6.15.44
-- Solivia.string1.current_max         6.15.45
-- Solivia.string1.power               6.15.46
-- Solivia.string1.power_min           6.15.47
-- Solivia.string1.power_max           6.15.48
-- 
-- Solivia.string2.voltage             6.16.40
-- Solivia.string2.voltage_min         6.16.41
-- Solivia.string2.voltage_max         6.16.42
-- Solivia.string2.current             6.16.43
-- Solivia.string2.current_min         6.16.44
-- Solivia.string2.current_max         6.16.45
-- Solivia.string2.power               6.16.46
-- Solivia.string2.power_min           6.16.47
-- Solivia.string2.power_max           6.16.48
-- 
-- Solivia.inverter.power              6.17.46
-- Solivia.inverter.power_min          6.17.47
-- Solivia.inverter.power_max          6.17.48
-- Solivia.inverter.feedtime           6.17.50
-- Solivia.inverter.energy             6.17.51
-- Solivia.inverter.temperature        6.17.52
-- Solivia.inverter.temperature_min    6.17.53
-- Solivia.inverter.temperature_max    6.17.54
-- 
-- 
