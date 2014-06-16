USE tdb;

# main tea listings
DROP TABLE IF EXISTS tea;
CREATE TABLE tea (
	TeaID INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
	TeaName VARCHAR(128) NOT NULL DEFAULT '',
	TeaTypeID INT NOT NULL
) ENGINE=InnoDB;

INSERT INTO tea (`TeaName`,`TeaTypeID`) VALUES
	('China Organic Green',1),
	('Shizuoka Houji-Cha',5),
	('Organic Oolong',3),
	('Ginseng Oolong',3);


# major categories
DROP TABLE IF EXISTS tea_type;
CREATE TABLE tea_type (
	TeaTypeID INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
	TeaType VARCHAR(128) NOT NULL DEFAULT ''
) ENGINE=InnoDB;

INSERT INTO tea_type (`TeaTypeID`,`TeaType`) VALUES
	(1,'green'),
	(2,'white'),
	(3,'oolong'),
	(4,'black'),
	(5,'roasted'),
	(6,'herbal');


# units
DROP TABLE IF EXISTS tea_unit;
CREATE TABLE tea_unit (
	TeaUnitID INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
	TeaUnit VARCHAR(128) NOT NULL DEFAULT ''
) ENGINE=InnoDB;

INSERT INTO tea_unit (`TeaUnitID`,`TeaUnit`) VALUES
	(1,'bag'),
	(2,'ounce'),
	(3,'gram');


# inventory
DROP TABLE IF EXISTS tea_inventory;
CREATE TABLE tea_inventory (
	TeaInventoryID INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
	TeaID INT NOT NULL,
	TeaUnitID INT NOT NULL,
	TeaQuantity DECIMAL(9,2) NOT NULL DEFAULT 0.00,
	TeaServingsPerUnit INT NOT NULL DEFAULT 0
) ENGINE=InnoDB;

INSERT INTO tea_inventory (`TeaID`,`TeaUnitID`,`TeaQuantity`,`TeaServingsPerUnit`) VALUES
	(1,1,45,1),
	(2,1,4,1),
	(3,1,21,1),
	(4,1,9,1);
