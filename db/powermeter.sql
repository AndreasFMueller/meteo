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
	values('urms_phase1_min', 1, 'V', 'VoltageValue');
insert into mfield(name, id, unit, class)
	values('urms_phase1_max', 2, 'V', 'VoltageValue');

insert into mfield(name, id, unit, class)
	values('urms_phase2', 3, 'V', 'VoltageValue');
insert into mfield(name, id, unit, class)
	values('urms_phase2_min', 4, 'V', 'VoltageValue');
insert into mfield(name, id, unit, class)
	values('urms_phase2_max', 5, 'V', 'VoltageValue');

insert into mfield(name, id, unit, class)
	values('urms_phase3', 6, 'V', 'VoltageValue');
insert into mfield(name, id, unit, class)
	values('urms_phase3_min', 7, 'V', 'VoltageValue');
insert into mfield(name, id, unit, class)
	values('urms_phase3_max', 8, 'V', 'VoltageValue');

insert into mfield(name, id, unit, class)
	values('irms_phase1', 9, 'A', 'CurrentValue');
insert into mfield(name, id, unit, class)
	values('irms_phase2', 10, 'A', 'CurrentValue');
insert into mfield(name, id, unit, class)
	values('irms_phase3', 11, 'A', 'CurrentValue');

insert into mfield(name, id, unit, class)
	values('prms_phase1', 12, 'W', 'PowerValue');
insert into mfield(name, id, unit, class)
	values('prms_phase1_min', 13, 'W', 'PowerValue');
insert into mfield(name, id, unit, class)
	values('prms_phase1_max', 14, 'W', 'PowerValue');

insert into mfield(name, id, unit, class)
	values('prms_phase2', 15, 'W', 'PowerValue');
insert into mfield(name, id, unit, class)
	values('prms_phase2_min', 16, 'W', 'PowerValue');
insert into mfield(name, id, unit, class)
	values('prms_phase2_max', 17, 'W', 'PowerValue');

insert into mfield(name, id, unit, class)
	values('prms_phase3', 18, 'W', 'PowerValue');
insert into mfield(name, id, unit, class)
	values('prms_phase3_min', 19, 'W', 'PowerValue');
insert into mfield(name, id, unit, class)
	values('prms_phase3_max', 20, 'W', 'PowerValue');

insert into mfield(name, id, unit)
	values('qrms_phase1', 21, '');
insert into mfield(name, id, unit)
	values('qrms_phase2', 22, '');
insert into mfield(name, id, unit)
	values('qrms_phase3', 23, '');

insert into mfield(name, id, unit)
	values('cosphi_phase1', 24, '');
insert into mfield(name, id, unit)
	values('cosphi_phase2', 25, '');
insert into mfield(name, id, unit)
	values('cosphi_phase3', 26, '');

insert into mfield(name, id, unit, class)
	values('prms_total', 27, 'W', 'PowerValue');
insert into mfield(name, id, unit, class)
	values('prms_total_min', 28, 'W', 'PowerValue');
insert into mfield(name, id, unit, class)
	values('prms_total_max', 29, 'W', 'PowerValue');

insert into mfield(name, id, unit)
	values('qrms_total', 30, '');
insert into mfield(name, id, unit)
	values('qrms_total_min', 31, '');
insert into mfield(name, id, unit)
	values('qrms_total_max', 32, '');



