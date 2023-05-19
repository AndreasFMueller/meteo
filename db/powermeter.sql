--
-- powermeter.sql
--
-- (c) 2023 Prof Dr Andreas Müller
--
insert into station(name, id, timezone, offset, longitude, latitude, altitude)
	values('Bubental', 5, 'MET', 3600, 47.18, 8.81, 610);

insert into sensor (name, id, stationid) values ('powermeter', 11, 5);

insert into mfield(name, id, unit, class)
	values('urms_phase1', 0, 'V', 'VoltageValue');
insert into mfield(name, id, unit, class)
	values('urms_phase2', 1, 'V', 'VoltageValue');
insert into mfield(name, id, unit, class)
	values('urms_phase3', 2, 'V', 'VoltageValue');

insert into mfield(name, id, unit, class)
	values('irms_phase1', 3, 'A', 'CurrentValue');
insert into mfield(name, id, unit, class)
	values('irms_phase2', 4, 'A', 'CurrentValue');
insert into mfield(name, id, unit, class)
	values('irms_phase3', 5, 'A', 'CurrentValue');

insert into mfield(name, id, unit, class)
	values('prms_phase1', 6, 'W', 'PowerValue');
insert into mfield(name, id, unit, class)
	values('prms_phase1_min', 7, 'W', 'PowerValue');
insert into mfield(name, id, unit, class)
	values('prms_phase1_max', 8, 'W', 'PowerValue');

insert into mfield(name, id, unit, class)
	values('prms_phase2', 9, 'W', 'PowerValue');
insert into mfield(name, id, unit, class)
	values('prms_phase2_min', 10, 'W', 'PowerValue');
insert into mfield(name, id, unit, class)
	values('prms_phase2_max', 11, 'W', 'PowerValue');

insert into mfield(name, id, unit, class)
	values('prms_phase3', 12, 'W', 'PowerValue');
insert into mfield(name, id, unit, class)
	values('prms_phase3_min', 13, 'W', 'PowerValue');
insert into mfield(name, id, unit, class)
	values('prms_phase3_max', 14, 'W', 'PowerValue');

insert into mfield(name, id, unit)
	values('qrms_phase1', 15, '');
insert into mfield(name, id, unit)
	values('qrms_phase2', 16, '');
insert into mfield(name, id, unit)
	values('qrms_phase3', 17, '');

insert into mfield(name, id, unit)
	values('cosphi_phase1', 18, '');
insert into mfield(name, id, unit)
	values('cosphi_phase2', 19, '');
insert into mfield(name, id, unit)
	values('cosphi_phase3', 20, '');

insert into mfield(name, id, unit, class)
	values('prms_total', 21, 'W', 'PowerValue');
insert into mfield(name, id, unit, class)
	values('prms_total_min', 22, 'W', 'PowerValue');
insert into mfield(name, id, unit, class)
	values('prms_total_max', 23, 'W', 'PowerValue');

insert into mfield(name, id, unit)
	values('qrms_total', 24, '');



