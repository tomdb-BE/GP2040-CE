import React from 'react';
import { useTranslation } from 'react-i18next';
import { FormCheck, Row } from 'react-bootstrap';
import * as yup from 'yup';

import Section from '../Components/Section';

import FormControl from '../Components/FormControl';
import FormSelect from '../Components/FormSelect';
import { BUTTON_MASKS } from '../Data/Buttons';

export const z680Scheme = {
	Z680AddonEnabled: yup
		.number()
		.required()
		.label('Z680 Enabled'),		
	z680PowerPin: yup
		.number()
		.label('Z680 Power Pin')
		.validatePinWhenValue('Z680AddonEnabled'),
	z680VolumeUpPin: yup
		.number()
		.label('Z680 Volume Up Pin')
		.validatePinWhenValue('Z680AddonEnabled'),
	z680VolumeDownPin: yup
		.number()
		.label('Z680 Volume Down Pin')
		.validatePinWhenValue('Z680AddonEnabled'),
	z680MutePin: yup
		.number()
		.label('Z680 Mute Pin')
		.validatePinWhenValue('Z680AddonEnabled'),
	z680PowerStatePin: yup
		.number()
		.label('Z680 Power State Pin')
		.validatePinWhenValue('Z680AddonEnabled'),
	z680ButtonMask: yup
		.number()
		.label('Z680 Button Mask')
		.validateSelectionWhenValue('Z680AddonEnabled', BUTTON_MASKS),			
};

export const z680State = {
	Z680AddonEnabled: 0,		
	z680PowerPin: -1,
	z680VolumeUpPin: -1,
	z680VolumeDownPin: -1,
	z680MutePin: -1,
	z680PowerStatePin: -1,
	z680ButtonMask: 0,
};

const Z680 = ({ values, errors, handleChange, handleCheckbox }) => {
	const { t } = useTranslation();
	return (
		<Section title={t('AddonsConfig:z680-header-text')}>
			<div
				id="Z680AddonOptions"
				hidden={!values.Z680AddonEnabled}
			>
				<Row className="mb-3">
					<FormSelect
						type="number"
						label={t('AddonsConfig:z680-button-mask-label')}
						name="z680ButtonMask"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.z680ButtonMask}
						error={errors.z680ButtonMask}
						isInvalid={errors.z680ButtonMask}
						onChange={handleChange}
					>
						{BUTTON_MASKS.map((o, i) => (
							<option key={`z680ButtonMask-option-${i}`} value={o.value}>
								{o.label}
							</option>
						))}
					</FormSelect>			
					<FormControl
						type="number"
						label={t('AddonsConfig:z680-power-pin-label')}
						name="z680PowerPin"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.z680PowerPin}
						error={errors.z680PowerPin}
						isInvalid={errors.z680PowerPin}
						onChange={handleChange}
						min={-1}
						max={29}
					/>								
					<FormControl
						type="number"
						label={t('AddonsConfig:z680-volume-up-pin-label')}
						name="z680VolumeUpPin"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.z680VolumeUpPin}
						error={errors.z680VolumeUpPin}
						isInvalid={errors.z680VolumeUpPin}
						onChange={handleChange}
						min={-1}
						max={29}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:z680-volume-down-pin-label')}
						name="z680VolumeDownPin"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.z680VolumeDownPin}
						error={errors.z680VolumeDownPin}
						isInvalid={errors.z680VolumeDownPin}
						onChange={handleChange}
						min={-1}
						max={29}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:z680-mute-pin-label')}
						name="z680MutePin"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.z680MutePin}
						error={errors.z680MutePin}
						isInvalid={errors.z680MutePin}
						onChange={handleChange}
						min={-1}
						max={29}
					/>
					<FormControl
						type="number"
						label={t('AddonsConfig:z680-power-state-pin-label')}
						name="z680PowerStatePin"
						className="form-control-sm"
						groupClassName="col-sm-3 mb-3"
						value={values.z680PowerStatePin}
						error={errors.z680PowerStatePin}
						isInvalid={errors.z680PowerStatePin}
						onChange={handleChange}
						min={-1}
						max={29}
					/>										
				</Row>		
			</div>
			<FormCheck
				label={t('Common:switch-enabled')}
				type="switch"
				id="Z680AddonButton"
				reverse
				isInvalid={false}
				checked={Boolean(values.Z680AddonEnabled)}
				onChange={(e) => {
					handleCheckbox('Z680AddonEnabled', values);
					handleChange(e);
				}}
			/>
		</Section>
	);
};

export default Z680;
