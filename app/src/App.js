import React, { useEffect, useState } from "react";
import logo from "./img/logo-mira.svg";
import "./App.css";

function App(props) {
  const [error, setError] = useState();
  const [animation, setAnimation] = useState();
  const [patterns, setPatterns] = useState();

  useEffect(() => {
    getAnimation();
    const interval = setInterval(() => {
      getAnimation();
    }, 1000);
    return () => {
      clearInterval(interval);
    };
  }, []);

  useEffect(() => {
    fetch("/api/patterns")
      .then((res) => res.json())
      .then((data) => {
        setPatterns(data);
      })
      .catch(function (err) {
        setError("No patterns found");
      });
  }, []);

  const getAnimation = () => {
    fetch("/api/animation")
      .then((res) => res.json())
      .then((data) => {
        setAnimation(data);
      })
      .catch(function (err) {
        setError("fetch failed");
      });
  };

  const updateAnimation = (id) => {
    fetch("/api/animation", {
      method: "PUT",
      body: JSON.stringify({
        id: id,
        brightness: 140,
      }),
    })
      .then((res) => res.json())
      .then((data) => {
        setAnimation(data);
      })
      .catch(function (err) {
        setError("fetch failed");
      });
  };

  if (error) {
    return <p>ERROR: {error}</p>;
  } else if (!animation || !patterns) {
    return <p>Loading...</p>;
  } else {
    let partyPatterns = [...patterns];
    partyPatterns.splice(0, 4);

    return (
      <div className="App">
        <header className="App-header">
          <p>
            MIRA running {animation.name} at {animation.brightness}
          </p>
          <img src={logo} className="App-logo" alt="logo" />
          <button onClick={() => updateAnimation(0)}>OFF</button>
          <button onClick={() => updateAnimation(1)}>STARGAZE</button>
          <button onClick={() => updateAnimation(2)}>RELAX</button>
          <button onClick={() => updateAnimation(4)}>PARTY</button>
          <button onClick={() => updateAnimation(3)}>REVEAL</button>

          {partyPatterns.map((item, key) => {
            return (
              <li key={key}>
                {item.id} - {item.name}
              </li>
            );
          })}
        </header>
      </div>
    );
  }
}

export default App;
