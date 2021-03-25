import React, { useEffect, useState } from "react";
import "./App.css";

function App(props) {
  const [error, setError] = useState();
  const [animation, setAnimation] = useState();
  const [patterns, setPatterns] = useState();

  useEffect(() => {
    getAnimation();
    const interval = setInterval(() => {
      getAnimation();
    }, 2000);
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
        setError("Oops, something went wrong");
      });
  }, []);

  const getAnimation = () => {
    fetch("/api/animation")
      .then((res) => res.json())
      .then((data) => {
        setAnimation(data);
      })
      .catch(function (err) {
        setError("Oops, something went wrong");
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
        setError("Oops, something went wrong");
      });
  };

  const logo = () => {
    return (
      <div>
        <div className="logo" aria-label="Mira"></div>
        <div className="logotype"></div>
      </div>
    );
  };

  const modeTab = (id) => {
    let modeClass = id !== 4 ? patterns[id].name.toLowerCase() : "party";
    return (
      <li
        aria-label={modeClass}
        onClick={() => updateAnimation(id)}
        className={`mode ${modeClass} ${animation.id === id ? "selected" : ""}`}
      >
        <div className="indicator"></div>
      </li>
    );
  };

  let activeMode = "loading";

  if (error) {
    return (
      <div className="message">
        {logo()}
        <p>
          {error}
          <br />
          <a href="/">Try again</a>
        </p>
      </div>
    );
  } else if (!animation || !patterns) {
    return (
      <div className="message">
        {logo()}
        <div className="loader"></div>
      </div>
    );
  } else {
    let partyPatterns = [...patterns];
    partyPatterns.splice(0, 4);
    activeMode = animation.id < 4 ? animation.name.toLowerCase() : "party";

    return (
      <div className={`panel ${activeMode}`}>
        {logo()}
        <div className={`controls stargaze`}></div>
        <div className={`controls relax`}></div>
        <div className={`controls party`}>
          <ul className="party-selector">
            {partyPatterns.map((item, key) => {
              return (
                <li
                  key={key}
                  className={`${animation.id === item.id ? "active" : ""}`}
                  onClick={() => updateAnimation(item.id)}
                  aria-label={item.name}
                >
                  {item.name}
                </li>
              );
            })}
          </ul>
        </div>
        <div className={`controls reveal`}></div>

        <div className="label">
          <h1>{activeMode}</h1>
        </div>

        <ul className="modes">
          {modeTab(0)}
          {modeTab(1)}
          {modeTab(2)}
          {modeTab(4)}
          {modeTab(3)}
        </ul>
      </div>
    );
  }
}

export default App;
