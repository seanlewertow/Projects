import styled from 'styled-components';

export const LeftSection = styled.div`
@media (max-width: 768px) {
  margin-left: 20px;
  margin-right: 20px;
}
  width: 100%;
  @media ${(props) => props.theme.breakpoints.sm} {
    width: 80%;
    display: flex;
    flex-direction: column;
    margin: 0 auto;
    padding: 0 20px; /* Add padding for smaller screens */
  }
  @media ${(props) => props.theme.breakpoints.md} {
    width: 100%;
    display: flex;
    flex-direction: column;
    margin: 0 auto;
  }
`;

