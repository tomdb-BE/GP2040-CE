import React from 'react';
import { useTranslation } from 'react-i18next';
import { FormCheck, Row } from 'react-bootstrap';
import * as yup from 'yup';

import Section from '../Components/Section';

import FormControl from '../Components/FormControl';

export const coinLedsScheme = {
	CoinLedsAddonEnabled: yup
		.number()
		.required()
		.label('Coin Leds Add-On Enabled'),
	coinLedsStartPin1: yup
		.number()
		.label('Start P1 LED Pin')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsStartPin2: yup
		.number()
		.label('Start P2 LED Pin')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsStartPin3: yup
		.number()
		.label('Start P3 LED Pin')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsStartPin4: yup
		.number()
		.label('Start P4 LED Pin')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsCoinPin1: yup
		.number()
		.label('Coin P1 LED Pin')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsCoinPin2: yup
		.number()
		.label('Coin P2 LED Pin')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsCoinPin3: yup
		.number()
		.label('Coin P3 LED Pin')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsCoinPin4: yup
		.number()
		.label('Coin P4 LED Pin')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsMarqueePin: yup
		.number()
		.label('Marquee LED Pin')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsExtStartPin: yup
		.number()
		.label('External Start Pin')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsExtCoinPin: yup
		.number()
		.label('External Coin Pin')
		.validatePinWhenValue('CoinLedsAddonEnabled'),		
	coinLedsStartBrightness: yup
		.number()
		.label('Start LED Brightness')
		.validateRangeWhenValue('CoinLedsAddonEnabled', 0, 100),
	coinLedsCoinBrightness: yup
		.number()
		.label('Coin LED Brightness')
		.validateRangeWhenValue('CoinLedsAddonEnabled', 0, 100),
	coinLedsMarqueeBrightness: yup
		.number()
		.label('Marquee LED Brightness')
		.validateRangeWhenValue('CoinLedsAddonEnabled', 0, 100),				
};

export const coinLedsState = {
	CoinLedsAddonEnabled: 0,
	coinLedsStartPin1: -1,
	coinLedsStartPin2: -1,
	coinLedsStartPin3: -1,
	coinLedsStartPin4: -1,
	coinLedsCoinPin1: -1,
	coinLedsCoinPin2: -1,
	coinLedsCoinPin3: -1,
	coinLedsCoinPin4: -1,
	coinLedsMarqueePin: -1,
	coinLedsExtStartPin: -1,
	coinLedsExtCoinPin: -1,
	coinLedsStartBrightness: 100,
	coinLedsCoinBrightness: 100,
	coinLedsMarqueeBrightness: 100,
};

const CoinLeds = ({ values, errors, handleChange, handleCheckbox }) => {
	const { t } = useTranslation();
	return (
		<Section title={t('AddonsConfig:coin-leds-header-text')}>
			<div
				id="CoinLedsAddonOptions"
				hidden={!values.CoinLedsAddonEnabled}
			>
				<Row className="mb-3">
					<FormControl
						type="number"
						label={t('AddonsConfig:coin-leds-start-pin-1-label')}
						name="coinLedsStartPin1"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsStartPin1}
						error={errors.coinLedsStartPin1}
						isInvalid={errors.coinLedsStartPin1}
						onChange={handleChange}
						min={-1}
						max={29}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:coin-leds-start-pin-2-label')}
						name="coinLedsStartPin2"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsStartPin2}
						error={errors.coinLedsStartPin2}
						isInvalid={errors.coinLedsStartPin2}
						onChange={handleChange}
						min={-1}
						max={29}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:coin-leds-start-pin-3-label')}
						name="coinLedsStartPin3"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsStartPin3}
						error={errors.coinLedsStartPin3}
						isInvalid={errors.coinLedsStartPin3}
						onChange={handleChange}
						min={-1}
						max={29}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:coin-leds-start-pin-4-label')}
						name="coinLedsStartPin4"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsStartPin4}
						error={errors.coinLedsStartPin4}
						isInvalid={errors.coinLedsStartPin4}
						onChange={handleChange}
						min={-1}
						max={29}
					/>
				</Row>
				<Row className="mb-3">					
					<FormControl
						type="number"
						label={t('AddonsConfig:coin-leds-start-brightness-label')}
						name="coinLedsStartBrightness"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsStartBrightness}
						error={errors.coinLedsStartBrightness}
						isInvalid={errors.coinLedsStartBrightness}
						onChange={handleChange}
						min={0}
						max={100}
					/>							
				</Row>
				<Row className="mb-3">
					<FormControl
						type="number"
						label={t('AddonsConfig:coin-leds-coin-pin-1-label')}
						name="coinLedsCoinPin1"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsCoinPin1}
						error={errors.coinLedsCoinPin1}
						isInvalid={errors.coinLedsCoinPin1}
						onChange={handleChange}
						min={-1}
						max={29}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:coin-leds-coin-pin-2-label')}
						name="coinLedsCoinPin2"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsCoinPin2}
						error={errors.coinLedsCoinPin2}
						isInvalid={errors.coinLedsCoinPin2}
						onChange={handleChange}
						min={-1}
						max={29}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:coin-leds-coin-pin-3-label')}
						name="coinLedsCoinPin3"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsCoinPin3}
						error={errors.coinLedsCoinPin3}
						isInvalid={errors.coinLedsCoinPin3}
						onChange={handleChange}
						min={-1}
						max={29}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:coin-leds-coin-pin-4-label')}
						name="coinLedsCoinPin4"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsCoinPin4}
						error={errors.coinLedsCoinPin4}
						isInvalid={errors.coinLedsCoinPin4}
						onChange={handleChange}
						min={-1}
						max={29}
					/>
				</Row>
				<Row className="mb-3">					
					<FormControl
						type="number"
						label={t('AddonsConfig:coin-leds-coin-brightness-label')}
						name="coinLedsCoinBrightness"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsCoinBrightness}
						error={errors.coinLedsCoinBrightness}
						isInvalid={errors.coinLedsCoinBrightness}
						onChange={handleChange}
						min={0}
						max={100}
					/>							
				</Row>				
				<Row className="mb-3">
					<FormControl
						type="number"
						label={t('AddonsConfig:coin-leds-marquee-pin-label')}
						name="coinLedsMarqueePin"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsMarqueePin}
						error={errors.coinLedsMarqueePin}
						isInvalid={errors.coinLedsMarqueePin}
						onChange={handleChange}
						min={-1}
						max={29}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:coin-leds-marquee-brightness-label')}
						name="coinLedsMarqueeBrightness"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsMarqueeBrightness}
						error={errors.coinLedsMarqueeBrightness}
						isInvalid={errors.coinLedsMarqueeBrightness}
						onChange={handleChange}
						min={0}
						max={100}
					/>					
				</Row>								
				<Row className="mb-3">					
					<FormControl
						type="number"
						label={t('AddonsConfig:coin-leds-ext-start-pin-label')}
						name="coinLedsExtStartPin"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsExtStartPin}
						error={errors.coinLedsExtStartPin}
						isInvalid={errors.coinLedsExtStartPin}
						onChange={handleChange}
						min={-1}
						max={29}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:coin-leds-ext-coin-pin-label')}
						name="coinLedsExtCoinPin"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsExtCoinPin}
						error={errors.coinLedsExtCoinPin}
						isInvalid={errors.coinLedsExtCoinPin}
						onChange={handleChange}
						min={-1}
						max={29}
					/>					
				</Row>	
			</div>
			<FormCheck
				label={t('Common:switch-enabled')}
				type="switch"
				id="CoinLedsAddonButton"
				reverse
				isInvalid={false}
				checked={Boolean(values.CoinLedsAddonEnabled)}
				onChange={(e) => {
					handleCheckbox('CoinLedsAddonEnabled', values);
					handleChange(e);
				}}
			/>
		</Section>
	);
};

export default CoinLeds;
