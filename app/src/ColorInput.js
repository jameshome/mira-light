import React, { createRef, useEffect, useRef } from "react";
import iro from "@jaames/iro";

const ColorInput = (props) => {
  let colorPicker = useRef(null);
  let layoutComponent =
    props.layout === "slider" ? iro.ui.Slider : iro.ui.Wheel;
  let el = createRef();
  useEffect(() => {
    if (!el.current) {
      return;
    }
    if (!colorPicker.current) {
      colorPicker.current = new iro.ColorPicker(el.current, {
        color: props.color,
        width: props.width,
        borderWidth: props.borderWidth,
        layout: [{ component: layoutComponent }],
      });

      if (!colorPicker.current) {
        return;
      }
      colorPicker.current.on("color:change", (color) => {
        if (props.onColorChange) {
          props.onColorChange(color);
        }
      });
    } else if (props.color !== colorPicker.current.color.hsv) {
      colorPicker.current.color.set(props.color);
    }
  });
  return React.createElement("div", { ref: el });
};
export default ColorInput;
