import React from 'react';
import { DiFirebase, DiReact, DiZend } from 'react-icons/di';
import { Section, SectionDivider, SectionText, SectionTitle } from '../../styles/GlobalComponents';
import { List, ListContainer, ListItem, ListParagraph, ListTitle } from './TechnologiesStyles';

const Technologies = () =>  (
  <Section id="tech">
    <SectionDivider />
    <br/>
    <SectionTitle>Technologies</SectionTitle>
    <SectionText>
      The range of technologies that I've worked with:
    </SectionText>
    <List>
      <ListItem>
        <DiFirebase size = "3rem" />
        <ListContainer>
        <ListTitle style={{ color: '#FF6F61' }}>Front-End</ListTitle>
        <ListParagraph>
          <span style={{ color: '#6B4226' }}>React.js</span>
        </ListParagraph>
        </ListContainer>
      </ListItem>
      <ListItem>
        <DiZend size = "3rem" />
        <ListContainer>
        <ListTitle style={{ color: '#5A6E6B' }}>Back-End</ListTitle>
        <ListParagraph>
          <span style={{ color: '#273238' }}>Python, C#, C++, SQL</span>
        </ListParagraph>
        </ListContainer>
      </ListItem>
      <ListItem>
        <DiReact size = "3rem" />
        <ListContainer>
        <ListTitle style={{ color: '#FFC857' }}>Tools and Frameworks</ListTitle>
        <ListParagraph>
          <span style={{ color: '#4A4A4A' }}>JIRA, .NET, MySQL</span>
        </ListParagraph>
        </ListContainer>
      </ListItem>
      
    </List>
  </Section>
);

export default Technologies;
