import { AppContext } from '../Contexts/AppContext';
import React, { useContext, useEffect, useState } from 'react';
import { Trans, useTranslation } from 'react-i18next';
import * as yup from 'yup';

import invert from 'lodash/invert';
import omit from 'lodash/omit';

import Section from '../Components/Section';
import FormSelect from '../Components/FormSelect';
import FormControl from '../Components/FormControl';
import { Button, FormCheck, Row, Col } from 'react-bootstrap';

import WebApi, { } from '../Services/WebApi';

import { BUTTON_MASKS_OPTIONS } from '../Data/Buttons';

const NON_SELECTABLE_BUTTONS_VALUE = [
	(1 << 31)
];

const NON_SELECTABLE_BUTTONS_LABEL = [
];

export const i2cMapperScheme = {
	I2CMapperAddonEnabled: yup
		.number()
		.required()
		.label('I2C Mapper Add-On Enabled'),
	
};

export const i2cMapperState = {
	I2CMapperAddonEnabled: 0,
};

const isNonSelectable = (button) =>
	NON_SELECTABLE_BUTTONS_LABEL.includes(button.label) || NON_SELECTABLE_BUTTONS_VALUE.includes(button.value);

const I2CMapper = ({ values, errors, handleChange, handleCheckbox }) => {
	const { t } = useTranslation();
	const [i2cMaps, setSelectedMaps] = useState([]);
	const [selectedMaps] = useState([]);
	const { setLoading } = useContext(AppContext);
	
	useEffect(() => {
		async function fetchData() {
			const i2cMaps = await WebApi.getI2CMaps(setLoading);
			setSelectedMaps(i2cMaps.maps);		
		}
		fetchData();	
	}, [setSelectedMaps, selectedMaps]);

	const handleMapChange = (c : any, idx : number, field : string, value : Number) => {
		c[idx][field] = value;
		console.dir(c);
		return [...c];
	};

	const saveMapSettings = async () => {
		const mapSettings = await WebApi.setI2CMaps({maps: i2cMaps});	
	};
	
	return (
		<Section title={t('I2CMapper:header-text')}>
			<div id="I2CMapperEnabledOptions" hidden={!values.I2CMapperAddonEnabled}>
				{i2cMaps?.length > 0 ? i2cMaps.map((o,i) => (	
					<div className="row card mb-3 me-1" id={`i2cMap${i}`} key={`i2cMap${i}`}>
						<Row className="mb-3">						
							<label className="mb-3">{t('I2CMapper:map-panel', { index: i + 1 })}</label>							
							<FormControl
								type="number"
								label={t('I2CMapper:map-address')}
								name={`map${i}address`}
								className="form-control-sm"
								groupClassName="col-sm-3 mb-3"
								value={i2cMaps[i].address}
								onChange={(e) => setSelectedMaps((c) => handleMapChange(c, i, 'address', Number(e.target.value)))}
								min={0}
								max={255}
							/>
							<FormControl
								type="number"
								label={t('I2CMapper:map-command-1')}
								name={`map${i}command1`}
								className="form-control-sm"
								groupClassName="col-sm-3 mb-3"
								value={i2cMaps[i].command1}
								onChange={(e) => setSelectedMaps((c) => handleMapChange(c, i, 'command1', Number(e.target.value)))}
								min={0}
								max={255}
							/>
							<FormControl
								type="number"
								label={t('I2CMapper:map-command-2')}
								name={`map${i}command2`}
								className="form-control-sm"
								groupClassName="col-sm-3 mb-3"
								value={i2cMaps[i].command2}
								onChange={(e) => setSelectedMaps((c) => handleMapChange(c, i, 'command2', Number(e.target.value)))}
								min={0}
								max={255}
							/>
							<FormControl
								type="number"
								label={t('I2CMapper:map-command-3')}
								name={`map${i}command3`}
								className="form-control-sm"
								groupClassName="col-sm-3 mb-3"
								value={i2cMaps[i].command3}
								onChange={(e) => setSelectedMaps((c) => handleMapChange(c, i, 'command3', Number(e.target.value)))}
								min={0}
								max={255}
							/>																								
							<FormSelect
								label={t('I2CMapper:map-buttonmask-1')}
								name={`map${i}buttonMask1`}
								className="form-select-sm"
								groupClassName="col-sm-3 mb-3"
								value={i2cMaps[i].buttonMask1}
								onChange={(e) => setSelectedMaps((c) => handleMapChange(c, i, 'buttonMask1', Number(e.target.value)))}
							>
								{Object.values(BUTTON_MASKS_OPTIONS).filter((o) => !isNonSelectable(o.label)).map((mo, mi) => (				
									<option key={`map${i}buttonMask1-option-${mi}`} value={mo.value>>>0}>
										{mo.label}
									</option>
								))}												
							</FormSelect>
							<FormSelect
								label={t('I2CMapper:map-buttonmask-2')}
								name={`map${i}buttonMask2`}
								className="form-select-sm"
								groupClassName="col-sm-3 mb-3"
								value={i2cMaps[i].buttonMask2}
								onChange={(e) => setSelectedMaps((c) => handleMapChange(c, i, 'buttonMask2', Number(e.target.value)))}
							>
								{Object.values(BUTTON_MASKS_OPTIONS).filter((o) => !isNonSelectable(o)).map((mo, mi) => (
									<option key={`map${i}buttonMask2-option-${mi}`} value={mo.value>>>0}>
										{mo.label}
									</option>
								))}									
							</FormSelect>							
						</Row>
					</div>
				)) : ''}					
				<Row className="mb-3">
					<div className="col-sm-3 mb-3">
						<Button type="button" onClick={saveMapSettings}>
							{t('I2CMapper:button-save')}
						</Button>
					</div>
				</Row>
			</div>
			<FormCheck
				label={t('Common:switch-enabled')}
				type="switch"
				id="I2CMapperAddonButton"
				reverse
				isInvalid={false}
				checked={Boolean(values.I2CMapperAddonEnabled)}
				onChange={(e) => {
					handleCheckbox('I2CMapperAddonEnabled', values);
					handleChange(e);
				}}
			/>
		</Section>
	);
};

export default I2CMapper;
