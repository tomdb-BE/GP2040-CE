import React from 'react';
import { useTranslation } from 'react-i18next';
import { FormCheck, Row } from 'react-bootstrap';
import * as yup from 'yup';

import Section from '../Components/Section';

import FormControl from '../Components/FormControl';
import FormSelect from '../Components/FormSelect';
import { BUTTON_MASKS_OPTIONS } from '../Data/Buttons';

export const pcControlScheme = {
	PcControlAddonEnabled: yup
		.boolean()
		.required()
		.label('PC Control Enabled'),		
	pcControlPowerPin: yup
		.number()
		.required()
		.label('PC Control Power Pin')
		.validatePinWhenValue('PcControlAddonEnabled'),
	pcControlSwitchMask: yup
		.number()
		.required()
		.label('PC Control Power Switch Mask')
		.validateSelectionWhenValue('PcControlAddonEnabled', BUTTON_MASKS_OPTIONS),
	pcControlButtonMask1: yup
		.number()
		.required()
		.label('PC Control Button Mask 1')
		.validateSelectionWhenValue('PcControlAddonEnabled', BUTTON_MASKS_OPTIONS),
	pcControlButtonMask2: yup
		.number()
		.required()
		.label('PC Control Button Mask 2')
		.validateSelectionWhenValue('PcControlAddonEnabled', BUTTON_MASKS_OPTIONS),
};

export const pcControlState = {
	PcControlAddonEnabled: 0,		
	pcControlPowerPin: -1,
	pcControlSwitchMask: 0,
	pcControlButtonMask1: 0,
	pcControlButtonMask2: 0,
};

const PcControl = ({ values, errors, handleChange, handleCheckbox }) => {
	const { t } = useTranslation();
	return (
		<Section title={t('AddonsConfig:pc-control-header-text')}>
			<div
				id="PcControlAddonOptions"
				hidden={!values.PcControlAddonEnabled}
			>
				<Row className="mb-3">
					<FormSelect
						type="number"
						label={t('AddonsConfig:pc-control-button-mask-1-label')}
						name="pcControlButtonMask1"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.pcControlButtonMask1}
						error={errors.pcControlButtonMask1}
						isInvalid={errors.pcControlButtonMask1}
						onChange={handleChange}
					>
						{BUTTON_MASKS_OPTIONS.map((o, i) => (
							<option key={`pcControlButtonMask-1-option-${i}`} value={o.value}>
								{o.label}
							</option>
						))}
					</FormSelect>
					<FormSelect
						type="number"
						label={t('AddonsConfig:pc-control-button-mask-2-label')}
						name="pcControlButtonMask2"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.pcControlButtonMask2}
						error={errors.pcControlButtonMask2}
						isInvalid={errors.pcControlButtonMask2}
						onChange={handleChange}
					>
						{BUTTON_MASKS_OPTIONS.map((o, i) => (
							<option key={`pcControlButtonMask-2-option-${i}`} value={o.value}>
								{o.label}
							</option>
						))}
					</FormSelect>
				</Row>
				<Row className="mb-3">							
					<FormSelect
						type="number"
						label={t('AddonsConfig:pc-control-switch-mask-label')}
						name="pcControlSwitchMask"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.pcControlSwitchMask}
						error={errors.pcControlSwitchMask}
						isInvalid={errors.pcControlSwitchMask}
						onChange={handleChange}
					>
						{BUTTON_MASKS_OPTIONS.map((o, i) => (
							<option key={`pcControlSwitchMask-option-${i}`} value={o.value}>
								{o.label}
							</option>
						))}
					</FormSelect>
					<FormControl
						type="number"
						label={t('AddonsConfig:pc-control-power-pin-label')}
						name="pcControlPowerPin"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.pcControlPowerPin}
						error={errors.pcControlPowerPin}
						isInvalid={errors.pcControlPowerPin}
						onChange={handleChange}
						min={-1}
						max={29}
					/>						
				</Row>		
			</div>
			<FormCheck
				label={t('Common:switch-enabled')}
				type="switch"
				id="PcControlAddonButton"
				reverse
				isInvalid={false}
				checked={Boolean(values.PcControlAddonEnabled)}
				onChange={(e) => {
					handleCheckbox('PcControlAddonEnabled', values);
					handleChange(e);
				}}
			/>
		</Section>
	);
};

export default PcControl;
