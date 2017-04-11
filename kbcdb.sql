-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema kbcdb
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema kbcdb
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `kbcdb` DEFAULT CHARACTER SET utf8 ;
USE `kbcdb` ;

-- -----------------------------------------------------
-- Table `kbcdb`.`userData`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `kbcdb`.`userData` (
  `userName` VARCHAR(32) NOT NULL,
  `highScore` INT UNSIGNED ZEROFILL NULL,
  PRIMARY KEY (`userName`),
  UNIQUE INDEX `userName_UNIQUE` (`userName` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `kbcdb`.`quesLevel1`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `kbcdb`.`quesLevel1` (
  `ques` VARCHAR(140) NOT NULL,
  `quesId` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `corrAns` VARCHAR(45) NOT NULL,
  `wrongAns1` VARCHAR(45) NOT NULL,
  `wrongAns2` VARCHAR(45) NOT NULL,
  `wrongAns3` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`quesId`),
  UNIQUE INDEX `quesId_UNIQUE` (`quesId` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `kbcdb`.`quesLevel2`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `kbcdb`.`quesLevel2` (
  `ques` VARCHAR(140) NOT NULL,
  `quesId` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `corrAns` VARCHAR(45) NOT NULL,
  `wrongAns1` VARCHAR(45) NOT NULL,
  `wrongAns2` VARCHAR(45) NOT NULL,
  `wrongAns3` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`quesId`),
  UNIQUE INDEX `quesId_UNIQUE` (`quesId` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `kbcdb`.`quesLevel3`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `kbcdb`.`quesLevel3` (
  `ques` VARCHAR(140) NOT NULL,
  `quesId` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `corrAns` VARCHAR(45) NOT NULL,
  `wrongAns1` VARCHAR(45) NOT NULL,
  `wrongAns2` VARCHAR(45) NOT NULL,
  `wrongAns3` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`quesId`),
  UNIQUE INDEX `quesId_UNIQUE` (`quesId` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `kbcdb`.`quesLevel4`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `kbcdb`.`quesLevel4` (
  `ques` VARCHAR(140) NOT NULL,
  `quesId` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `corrAns` VARCHAR(45) NOT NULL,
  `wrongAns1` VARCHAR(45) NOT NULL,
  `wrongAns2` VARCHAR(45) NOT NULL,
  `wrongAns3` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`quesId`),
  UNIQUE INDEX `quesId_UNIQUE` (`quesId` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `kbcdb`.`quesLevel5`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `kbcdb`.`quesLevel5` (
  `ques` VARCHAR(140) NOT NULL,
  `quesId` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `corrAns` VARCHAR(45) NOT NULL,
  `wrongAns1` VARCHAR(45) NOT NULL,
  `wrongAns2` VARCHAR(45) NOT NULL,
  `wrongAns3` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`quesId`),
  UNIQUE INDEX `quesId_UNIQUE` (`quesId` ASC))
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
