import React from 'react';

import { Section, SectionDivider, SectionTitle } from '../../styles/GlobalComponents';
import { Box, Boxes, BoxNum, BoxText } from './AcomplishmentsStyles';

const data = [
  { number: '3', text: 'Academic Scholarships'},
  { number: '2000+', text: 'Lines of Code Written'},
  { number: '80+', text: 'University Volunteer Hours', },
  { number: '10+', text: 'Coding Projects Completed', }
];

const Acomplishments = () => (
  <Section>
    <SectionDivider />
    <br/>
    <SectionTitle>Personal Acomplishments</SectionTitle>
    <Boxes>
      {data.map((card, index) => (
        <Box key={index}>
          <BoxNum>{card.number}</BoxNum>
          <BoxText>{card.text}</BoxText>
        </Box>
      ))}
    </Boxes>
  </Section>
)

export default Acomplishments;
