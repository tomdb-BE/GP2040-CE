import React from 'react';
import { useTranslation } from 'react-i18next';
import { FormCheck, Row } from 'react-bootstrap';
import * as yup from 'yup';

import Section from '../Components/Section';

import FormControl from '../Components/FormControl';
import FormSelect from '../Components/FormSelect';
import { BUTTON_MASKS_OPTIONS } from '../Data/Buttons';

export const coinLedsScheme = {
	CoinLedsAddonEnabled: yup
		.boolean()
		.required()
		.label('Coin Leds Enabled'),
	coinLedsStartPin1: yup
		.number()
		.required()
		.label('Start P1 LED Pin')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsStartPin2: yup
		.number()
		.required()
		.label('Start P2 LED Pin')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsStartPin3: yup
		.number()
		.required()
		.label('Start P3 LED Pin')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsStartPin4: yup
		.number()
		.required()
		.label('Start P4 LED Pin')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsCoinPin1: yup
		.number()
		.required()
		.label('Coin P1 LED Pin')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsCoinPin2: yup
		.number()
		.required()
		.label('Coin P2 LED Pin')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsCoinPin3: yup
		.number()
		.required()
		.label('Coin P3 LED Pin')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsCoinPin4: yup
		.number()
		.required()
		.label('Coin P4 LED Pin')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsMarqueePin: yup
		.number()
		.required()
		.label('Marquee LED Pin')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsExtStartPinOut: yup
		.number()
		.required()
		.label('External Start Pin Out')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsExtCoinPinOut: yup
		.number()
		.required()
		.label('External Coin Pin Out')
		.validatePinWhenValue('CoinLedsAddonEnabled'),
	coinLedsStartMask1: yup
		.number()
		.required()
		.label('Start Mask 1')
		.validateSelectionWhenValue('CoinLedsAddonEnabled', BUTTON_MASKS_OPTIONS),
	coinLedsStartMask2: yup
		.number()
		.required()
		.label('Start Mask 2')
		.validateSelectionWhenValue('CoinLedsAddonEnabled', BUTTON_MASKS_OPTIONS),
	coinLedsStartMask3: yup
		.number()
		.required()
		.label('Start Mask 3')
		.validateSelectionWhenValue('CoinLedsAddonEnabled', BUTTON_MASKS_OPTIONS),
	coinLedsStartMask4: yup
		.number()
		.required()
		.label('Start Mask 4')
		.validateSelectionWhenValue('CoinLedsAddonEnabled', BUTTON_MASKS_OPTIONS),						
	coinLedsExtStartMask: yup
		.number()
		.required()
		.label('External Start Mask')
		.validateSelectionWhenValue('CoinLedsAddonEnabled', BUTTON_MASKS_OPTIONS),
	coinLedsCoinMask1: yup
		.number()
		.required()
		.label('Coin Mask 1')
		.validateSelectionWhenValue('CoinLedsAddonEnabled', BUTTON_MASKS_OPTIONS),
	coinLedsCoinMask2: yup
		.number()
		.required()
		.label('Coin Mask 2')
		.validateSelectionWhenValue('CoinLedsAddonEnabled', BUTTON_MASKS_OPTIONS),
	coinLedsCoinMask3: yup
		.number()
		.required()
		.label('Coin Mask 3')
		.validateSelectionWhenValue('CoinLedsAddonEnabled', BUTTON_MASKS_OPTIONS),
	coinLedsCoinMask4: yup
		.number()
		.required()
		.label('Coin Mask 4')
		.validateSelectionWhenValue('CoinLedsAddonEnabled', BUTTON_MASKS_OPTIONS),				
	coinLedsExtCoinMask: yup
		.number()
		.required()
		.label('External Coin Mask')
		.validateSelectionWhenValue('CoinLedsAddonEnabled', BUTTON_MASKS_OPTIONS),
	coinLedsStartBrightness: yup
		.number()
		.required()
		.label('Start LED Brightness')
		.validateRangeWhenValue('CoinLedsAddonEnabled', 0, 100),
	coinLedsCoinBrightness: yup
		.number()
		.required()
		.label('Coin LED Brightness')
		.validateRangeWhenValue('CoinLedsAddonEnabled', 0, 100),
	coinLedsMarqueeBrightness: yup
		.number()
		.required()
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
	coinLedsExtStartPinOut: -1,	
	coinLedsExtCoinPinOut: -1,
	coinLedsExtStartMask: 0,
	coinLedsExtCoinMask: 0,
	coinLedsStartMask1: 0,
	coinLedsStartMask2: 0,
	coinLedsStartMask3: 0,
	coinLedsStartMask4: 0,
	coinLedsCoinMask1: 0,
	coinLedsCoinMask2: 0,
	coinLedsCoinMask3: 0,
	coinLedsCoinMask4: 0,
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
					<FormSelect
						type="number"
						label={t('AddonsConfig:coin-leds-start-mask-1-label')}
						name="coinLedsStartMask1"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsStartMask1}
						error={errors.coinLedsStartMask1}
						isInvalid={errors.coinLedsStartMask1}
						onChange={handleChange}
					>
						{BUTTON_MASKS_OPTIONS.map((o, i) => (
							<option key={`coinLedsStartMask1-option-${i}`} value={o.value}>
								{o.label}
							</option>
						))}
					</FormSelect>			
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
					<FormSelect
						type="number"
						label={t('AddonsConfig:coin-leds-coin-mask-1-label')}
						name="coinLedsCoinMask1"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsCoinMask1}
						error={errors.coinLedsCoinMask1}
						isInvalid={errors.coinLedsCoinMask1}
						onChange={handleChange}
					>
						{BUTTON_MASKS_OPTIONS.map((o, i) => (
							<option key={`coinLedsCoinMask1-option-${i}`} value={o.value}>
								{o.label}
							</option>
						))}					
					</FormSelect>					
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
				</Row>
				<Row className="mb-3">
					<FormSelect
						type="number"
						label={t('AddonsConfig:coin-leds-start-mask-2-label')}
						name="coinLedsStartMask2"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsStartMask2}
						error={errors.coinLedsStartMask2}
						isInvalid={errors.coinLedsStartMask2}
						onChange={handleChange}
					>
						{BUTTON_MASKS_OPTIONS.map((o, i) => (
							<option key={`coinLedsStartMask2-option-${i}`} value={o.value}>
								{o.label}
							</option>
						))}
					</FormSelect>			
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
					<FormSelect
						type="number"
						label={t('AddonsConfig:coin-leds-coin-mask-2-label')}
						name="coinLedsCoinMask2"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsCoinMask2}
						error={errors.coinLedsCoinMask2}
						isInvalid={errors.coinLedsCoinMask2}
						onChange={handleChange}
					>
						{BUTTON_MASKS_OPTIONS.map((o, i) => (
							<option key={`coinLedsCoinMask2-option-${i}`} value={o.value}>
								{o.label}
							</option>
						))}					
					</FormSelect>					
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
				</Row>
				<Row className="mb-3">
					<FormSelect
						type="number"
						label={t('AddonsConfig:coin-leds-start-mask-3-label')}
						name="coinLedsStartMask3"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsStartMask3}
						error={errors.coinLedsStartMask3}
						isInvalid={errors.coinLedsStartMask3}
						onChange={handleChange}
					>
						{BUTTON_MASKS_OPTIONS.map((o, i) => (
							<option key={`coinLedsStartMask3-option-${i}`} value={o.value}>
								{o.label}
							</option>
						))}
					</FormSelect>			
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
					<FormSelect
						type="number"
						label={t('AddonsConfig:coin-leds-coin-mask-3-label')}
						name="coinLedsCoinMask3"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsCoinMask3}
						error={errors.coinLedsCoinMask3}
						isInvalid={errors.coinLedsCoinMask3}
						onChange={handleChange}
					>
						{BUTTON_MASKS_OPTIONS.map((o, i) => (
							<option key={`coinLedsCoinMask3-option-${i}`} value={o.value}>
								{o.label}
							</option>
						))}					
					</FormSelect>					
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
				</Row>
				<Row className="mb-3">
					<FormSelect
						type="number"
						label={t('AddonsConfig:coin-leds-start-mask-4-label')}
						name="coinLedsStartMask4"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsStartMask4}
						error={errors.coinLedsStartMask4}
						isInvalid={errors.coinLedsStartMask4}
						onChange={handleChange}
					>
						{BUTTON_MASKS_OPTIONS.map((o, i) => (
							<option key={`coinLedsStartMask4-option-${i}`} value={o.value}>
								{o.label}
							</option>
						))}
					</FormSelect>			
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
					<FormSelect
						type="number"
						label={t('AddonsConfig:coin-leds-coin-mask-4-label')}
						name="coinLedsCoinMask4"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsCoinMask4}
						error={errors.coinLedsCoinMask4}
						isInvalid={errors.coinLedsCoinMask4}
						onChange={handleChange}
					>
						{BUTTON_MASKS_OPTIONS.map((o, i) => (
							<option key={`coinLedsCoinMask4-option-${i}`} value={o.value}>
								{o.label}
							</option>
						))}					
					</FormSelect>					
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
					<FormSelect
						type="number"
						label={t('AddonsConfig:coin-leds-ext-start-mask-label')}
						name="coinLedsExtStartMask"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsExtStartMask}
						error={errors.coinLedsExtStartMask}
						isInvalid={errors.coinLedsExtStartMask}
						onChange={handleChange}
					>
						{BUTTON_MASKS_OPTIONS.map((o, i) => (
							<option key={`coinLedsExtStartMask-option-${i}`} value={o.value}>
								{o.label}
							</option>
						))}
					</FormSelect>					
					<FormControl
						type="number"
						label={t('AddonsConfig:coin-leds-ext-start-pin-out-label')}
						name="coinLedsExtStartPinOut"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsExtStartPinOut}
						error={errors.coinLedsExtStartPinOut}
						isInvalid={errors.coinLedsExtStartPinOut}
						onChange={handleChange}
						min={-1}
						max={29}
					/>					
				</Row>								
				<Row className="mb-3">					
					<FormSelect
						type="number"
						label={t('AddonsConfig:coin-leds-ext-coin-mask-label')}
						name="coinLedsExtCoinMask"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsExtCoinMask}
						error={errors.coinLedsExtCoinMask}
						isInvalid={errors.coinLedsExtCoinMask}
						onChange={handleChange}
						>
						{BUTTON_MASKS_OPTIONS.map((o, i) => (
							<option key={`coinLedsExtCoinMask-option-${i}`} value={o.value}>
								{o.label}
							</option>
						))}
					</FormSelect>
					<FormControl
						type="number"
						label={t('AddonsConfig:coin-leds-ext-coin-pin-out-label')}
						name="coinLedsExtCoinPinOut"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.coinLedsExtCoinPinOut}
						error={errors.coinLedsExtCoinPinOut}
						isInvalid={errors.coinLedsExtCoinPinOut}
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
