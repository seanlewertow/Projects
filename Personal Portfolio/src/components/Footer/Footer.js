import React from 'react';
import { AiFillGithub, AiFillInstagram, AiFillLinkedin } from 'react-icons/ai';

import { SocialIcons } from '../Header/HeaderStyles';
import { CompanyContainer, FooterWrapper, LinkColumn, LinkItem, LinkList, LinkTitle, Slogan, SocialContainer, SocialIconsContainer } from './FooterStyles';

const Footer = () => {
  const emails = [
    { type: 'Primary', address: 'seanlewertow@gmail.com' },
    { type: 'School', address: 's.lewertow@ufl.edu' },
  ];

  return (
    <FooterWrapper>
      <LinkList>
        <LinkColumn>
          <LinkTitle>Call</LinkTitle>
          <LinkItem href="tel:786-575-8386">(786)575-8386</LinkItem>
        </LinkColumn>
        {emails.map((email, index) => (
          <LinkColumn key={index}>
            <LinkTitle>{`${email.type} Email`}</LinkTitle>
            <a className='email' href={`mailto:${email.address}`} target="_blank" rel="noreferrer">{email.address}</a>
          </LinkColumn>
        ))}
      </LinkList>
      <SocialIconsContainer>
        <CompanyContainer>
          <Slogan>
            Black is S.T.E.M.
          </Slogan>
        </CompanyContainer>
        <SocialContainer>
          <SocialIcons href="https://github.com/seanlewertow">
            <AiFillGithub size="3rem" />
          </SocialIcons>
          <SocialIcons href="https://www.linkedin.com/in/seanlewertow/">
            <AiFillLinkedin size="3rem" />
          </SocialIcons>
          <SocialIcons href="https://www.instagram.com/ysk.sean/">
            <AiFillInstagram size="3rem" />
          </SocialIcons>
        </SocialContainer>
      </SocialIconsContainer>
    </FooterWrapper>
  );
};

export default Footer;
