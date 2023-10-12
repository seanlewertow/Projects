import React, { useState, useEffect } from 'react';
import styled from 'styled-components'; // Import styled-components

import { Section, SectionText, SectionTitle } from '../../styles/GlobalComponents';
import { LeftSection } from './HeroStyles';

const Cursor = styled.span`
  border-right: 2px solid #FF6F61; /* Adjust cursor color and size */
  animation: blink 0.7s infinite; /* Cursor animation */
  @keyframes blink {
    0%, 100% { border-color: transparent; }
    50% { border-color: #FF6F61; }
  }
`;

const ImageContainer = styled.div`
  position: fixed;
  bottom: 0.1;
  top: 3;
  left: 65%;
  transform: scaleX(1);
  max-width: 900px;
  max-height: 700px;
  width: 100%;
  height: auto;
  z-index: -1; /* Bring the image behind text */
  transition: opacity 10s ease;
  opacity: ${({ isVisible }) => (isVisible ? '1' : '0')};
`;

const Image = styled.img`
  object-fit: cover;
  width: 100%;
  height: 100%;
  border-radius: 50%;
  mix-blend-mode: screen;
`;

const Hero = (props) => {
  const [title, setTitle] = useState('');
  const [isVisible, setIsVisible] = useState(true);

  useEffect(() => {
    const handleScroll = () => {
      const scrollPosition = window.scrollY;
  
      if (scrollPosition > 200 && isVisible) {
        setIsVisible(false);
      } else if (scrollPosition <= 200 && !isVisible) {
        setIsVisible(true);
      }
    };
  
    const timeout = setTimeout(() => {
      window.addEventListener('scroll', handleScroll);
    }, 1000);
  
    return () => {
      clearTimeout(timeout);
      window.removeEventListener('scroll', handleScroll);
    };
  }, [isVisible]);
  

  useEffect(() => {
    const text = 'cout << "Hello to Sean\'s World";';
    let index = 0;

    const interval = setInterval(() => {
      setTitle(prevTitle => prevTitle + text[index]);
      index++;
      if (index === text.length) clearInterval(interval);
    }, 200);

    return () => clearInterval(interval);
  }, []);

  return (
    <Section row nopadding>
      <LeftSection style={{ marginLeft: '20px', marginRight: '20px' }}>
        <SectionTitle main center style={{ wordWrap: 'break-word' }}>
          {title}<Cursor>&nbsp;</Cursor>
        </SectionTitle>
        <SectionText style={{ textAlign: 'center', wordWrap: 'break-word' }}>
          When I'm not binge watching football or basketball, this is where you'll discover the fruits of my labor.
        </SectionText>
        <Section>
          <ImageContainer isVisible={isVisible}>
            <Image src="/images/Side.png" alt="Your Headshot" style={{ width: '100%', borderRadius: '50%', mixBlendMode: 'screen' }} />
          </ImageContainer>
          <div style={{ position: 'relative', top: '-50px' }}></div>
        </Section>
      </LeftSection>
    </Section>
  );
};

export default Hero;