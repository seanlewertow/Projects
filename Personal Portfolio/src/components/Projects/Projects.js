import React, { useState, useEffect, useRef } from 'react';
import {
  BlogCard,
  CardInfo,
  ExternalLinks,
  GridContainer,
  HeaderThree,
  Hr,
  Tag,
  TagList,
  TitleContent,
  UtilityList,
  Img
} from './ProjectsStyles';
import { Section, SectionDivider, SectionTitle } from '../../styles/GlobalComponents';
import { projects } from '../../constants/constants';

const Projects = () => {
  const [isScrolled, setIsScrolled] = useState(false);
  const projectsRef = useRef();

  useEffect(() => {
    const checkScroll = () => {
      const rect = projectsRef.current.getBoundingClientRect();
      setIsScrolled(rect.top <= window.innerHeight && rect.bottom >= 0);
    };

    window.addEventListener('scroll', checkScroll);
    return () => {
      window.removeEventListener('scroll', checkScroll);
    };
  }, []);

  return (
    <Section id="projects" ref={projectsRef} style={{ marginTop: '50px', filter: isScrolled ? 'none' : 'blur(8px)' }}>
      <SectionDivider />
      <SectionTitle main center>
        Projects
      </SectionTitle>
      <GridContainer>
        {projects.map(({ id, image, title, description, tags, source, Video, Report }) => (
          <BlogCard key={id}>
            <Img src={image} alt={title} />
            <TitleContent>
              <HeaderThree style={{ marginTop: '10px' }} title>{title}</HeaderThree>
              <Hr />
            </TitleContent>
            <CardInfo style={{ marginBottom: '20px' }}>
              {description}
            </CardInfo>
            <div>
              <TitleContent>Stack</TitleContent>
              <TagList>
                {tags.map((tag, i) => (
                  <Tag key={i}>{tag}</Tag>
                ))}
              </TagList>
            </div>
            <UtilityList>
              {Video && <ExternalLinks href={Video}>Video</ExternalLinks>}
              {Report && <ExternalLinks href={Report}>Report</ExternalLinks>}
              {source && <ExternalLinks href={source}>Source</ExternalLinks>}
            </UtilityList>
          </BlogCard>
        ))}
      </GridContainer>
    </Section>
  );
};

export default Projects;
