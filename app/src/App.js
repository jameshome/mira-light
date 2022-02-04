import React, { useEffect, useState } from "react";
import axios from "axios";
import { useDebounce } from "use-debounce";
import ColorInput from "./ColorInput";
import "./App.css";

export default function App(props) {
  // API calls
  const getAnimation = async () => {
    setError(false);
    try {
      const result = await axios("/api/animation");
      setAnimation(result.data);
    } catch (error) {
      setError(true);
    }
  };

  const putAnimation = async (settings) => {
    setError(false);
    try {
      await axios.put("/api/animation", settings);
    } catch (error) {
      setError(true);
    }
  };

  const changePattern = async (id) => {
    setError(false);
    try {
      await axios.put("/api/animation", { id: id });
      getAnimation();
    } catch (error) {
      setError(true);
    }
  };

  // Event & state handling
  const [error, setError] = useState(false);
  const [patterns, setPatterns] = useState([]);
  const [animation, setAnimation] = useState([]);

  useEffect(() => {
    const getPatterns = async () => {
      setError(false);
      try {
        const result = await axios("/api/patterns");
        setPatterns(result.data);
      } catch (error) {
        setError(true);
      }
    };
    getPatterns();
  }, []);

  useEffect(() => {
    getAnimation();
  }, []);

  // Convert between ColorPicker and FastLED HSV representations
  const byteToPercent = (value) => {
    return Math.round((value * 100) / 255);
  };

  const percentToByte = (value) => {
    return Math.round((value * 255) / 100);
  };

  const byteToDegree = (value) => {
    return Math.round((value * 360) / 255);
  };

  const degreeToByte = (value) => {
    return Math.round((value * 255) / 360);
  };

  // Render logo
  const logo = (mode) => {
    let appearance = !mode ? "expanded" : "collapsed";

    if (mode === "off") {
      appearance = "dimmed";
    }

    return (
      <a href="/">
        <div className={`logo ${appearance}`} aria-label="Mira">
          <div className="logotype"></div>
          <div className="refresh"></div>
        </div>
      </a>
    );
  };

  // Component: Navigation
  const Nav = (props) => {
    return (
      <nav>
        <div className="label">
          <h1>{props.mode}</h1>
        </div>
        <div className="modes">
          {props.modes.map((item, key) => {
            let selected =
              item.name.toLowerCase() === props.mode ? true : false;
            return (
              <Tab
                key={key}
                id={item.id}
                name={item.name.toLowerCase()}
                selected={selected}
              />
            );
          })}
        </div>
      </nav>
    );
  };

  // Component: Navigation Tab
  const Tab = (props) => {
    let selected = props.selected ? "selected" : "";
    return (
      <button
        className={`mode ${props.name} ${selected}`}
        onClick={(e) => changePattern(props.id)}
      >
        <div className="indicator"></div>
      </button>
    );
  };

  // Component: Brightness Control
  const Brightness = (props) => {
    const [brightness, setBrightness] = useState(
      byteToPercent(props.animation.brightness)
    );
    const [debouncedBrightness] = useDebounce(brightness, 200);

    useEffect(() => {
      let settings = { brightness: percentToByte(debouncedBrightness) };

      putAnimation(settings);
    }, [debouncedBrightness]);

    return (
      <div className="control-brightness">
        <ColorInput
          layout={"slider"}
          color={{
            h: 0,
            s: 0,
            v: brightness,
          }}
          width={270}
          borderWidth={2}
          onColorChange={(color) => {
            setBrightness(color.value);
          }}
        ></ColorInput>
      </div>
    );
  };

  // Component: Color Control
  const Color = (props) => {
    const [hue, setHue] = useState(byteToDegree(props.animation.hue));
    const [saturation, setSaturation] = useState(
      byteToPercent(props.animation.saturation)
    );
    const [debouncedHue] = useDebounce(hue, 200);
    const [debouncedSaturation] = useDebounce(saturation, 200);

    useEffect(() => {
      let settings = {
        hue: degreeToByte(debouncedHue),
        saturation: percentToByte(debouncedSaturation),
      };
      putAnimation(settings);
    }, [debouncedHue, debouncedSaturation]);

    return (
      <div className="control-color">
        <ColorInput
          layout={"wheel"}
          color={{
            h: hue,
            s: saturation,
            v: 100,
          }}
          width={270}
          borderWidth={2}
          onColorChange={(color) => {
            setHue(color.hue);
            setSaturation(color.saturation);
          }}
        ></ColorInput>
      </div>
    );
  };

  // Component: Patterns List
  const Patterns = (props) => {
    return (
      <div className="control-party">
        {props.patterns.map((item, key) => {
          return (
            <button
              key={item.id}
              className={`${animation.id === item.id ? "selected" : ""}`}
              onClick={() => changePattern(item.id)}
              aria-label={item.name}
            >
              {item.name}
              <div className="indicator"></div>
            </button>
          );
        })}
      </div>
    );
  };

  // Component: Controls
  const Controls = (props) => {
    switch (props.mode) {
      case "stargaze":
      case "relax":
      case "daze":
        return (
          <div className="controls">
            <Brightness animation={props.animation} />
            <Color animation={props.animation} />
          </div>
        );
      case "party":
        return (
          <div className="controls">
            <Brightness animation={props.animation} />
            <Patterns animation={props.animation} patterns={props.patterns} />
          </div>
        );

      default:
        return null;
    }
  };

  if (error) {
    return (
      <div className="message">
        {logo()}
        <p>
          Something went wrong
          <br />
          <a href="/">Try again</a>
        </p>
      </div>
    );
  } else if (!animation.name || !patterns) {
    // Render loading screen
    return (
      <div className="message">
        {logo()}
        <div className="loader"></div>
      </div>
    );
  } else {
    // the first four Patterns are Modes
    // the remaining Patterns are Party Patterns
    const partyPatterns = [...patterns];
    const modes = partyPatterns.splice(0, 4);
    modes.splice(3, 0, { id: 4, name: "Party" });
    // The current Mode is one of the first four Patterns or "party"
    let currentMode = animation.id < 4 ? animation.name.toLowerCase() : "party";

    return (
      <div className={`panel`}>
        {logo(currentMode)}
        <Controls
          mode={currentMode}
          animation={animation}
          patterns={partyPatterns}
        />
        <Nav mode={currentMode} modes={modes} />
      </div>
    );
  }
}
