"use strict";
(() => {
	$(document).ready(() => {
/** A LIST OF ALL CONSTANTS */

	// Session Storage Keys
	const currentModeKey = "UserCurrentMode";

	// Local Storage Keys
	const songObjectKey = "CachedSong";
	const autoSaveEnabledKey = "AutoSaveEnabled";
	const autoSaveIntervalKey = "AutoSaveInterval";
	const fontTypeKey = "FontType";
	const darkModeKey = "DarkModeMakerStudio";

	// Session Storage Values
	const selectSongModeSS = "SelectionMode";
	const editSongModeSS = "EditingMode";

	// Local Storage Values
	const autoSaveIntervalDefault = 5; // In Minutes

	// jQuery Elements on the DOM
	const selectSongModeTag = jQuery("#selectionMode");
	const editSongModeTag = jQuery("#editMode");
	const newFileButton = jQuery("#newFile");
	const getFileButton = jQuery("#getFile");
	const fontIncreaseButton = jQuery("#fontIncreaseButton");
	const fontDecreaseButton = jQuery("#fontDecreaseButton");
	const toggleDarkModeLink = jQuery("#toggleDark");
	const changeFontLink = jQuery("#changeFont");
	const resumeFileButton = jQuery("#resumeFile");
	const returnSelectModeLink = jQuery("#returnSelectMode");
	const songDataTable = jQuery("#songDataTable");
	const attrSongName = jQuery("#attr_songName");
	const attrSongSize = jQuery("#attr_songSize");
	const attrAuthor = jQuery("#attr_songAuthor");
	const attrToneLength = jQuery("#attr_toneLength");
	const attrToneDelay = jQuery("#attr_toneDelay");
	const attrCreationDate = jQuery("#attr_creationDate");
	const attrLastEditDate = jQuery("#attr_lastEditDate");
	const autoSaveMessageSummaryHeader = jQuery("#autoSaveMessage");
	const autoSaveMessage = jQuery("#autoSaveMessageText");
	const autoSaveSettingsEnabledCheckbox = jQuery("#autoSaveEnabled");
	const autoSaveSettingsInterval = jQuery("#autoSaveInterval");
	const autoSaveSettingsSaveLink = jQuery("#autoSaveSettingsSaveLink");
	const songPlaybackStatusLabel = jQuery("#playingLabel");
	const songProgressBar = jQuery("#songProgressBar");
	const currentSongProgressTime = jQuery("#currentProgressTime");
	const endingSongProgressTime = jQuery("#endingTime");
	const skipBackSongPlaybackButton = jQuery("#skipBack");
	const skipForwardSongPlaybackButton = jQuery("#skipForward");
	const playPauseSongPlaybackButton = jQuery("#playPauseSong");
	const restartSongPlaybackButton = jQuery("#restartSong");
	const stopSongPlaybackButton = jQuery("#stopSong");
	const songVolumeSlider = jQuery("#songVolume");
	const downloadSongButton = jQuery("#downloadBtn");
	const copySongDataButton = jQuery("#copyBtn");
	const restartSongButton = jQuery("#restartBtn");
	// Two different parts of instructions. One for select mode and one for editing mode.
	const userInstructionsSelectMode = jQuery("#selectModeInstructions");
	const userInstructionsEditingMode = jQuery("#editingModeInstructions");
	const editingModeInstructionContentMenu = jQuery("#editingModeInstructionContentMenu");
	const editingModeInstructionsContent = jQuery("#editingModeInstructionsContent");
	// Links for various content headers in the editing mode instructions.
	const iQuickStartLink = jQuery("#i01");
	const iAutoSaveLink = jQuery("#i02");
	const iUICustomizationLink = jQuery("#i03");
	const iEditingSongAttrLink = jQuery("#i04");
	const iEditingSongDataLink = jQuery("#i05");
	const iSongPlaybackLink = jQuery("#i06");
	const iDownloadUploadLink = jQuery("#i07");
	// The jQuery elements for the HTML content of each of the links.
	const iQuickStartContent = jQuery("#i01_c");
	const iAutoSaveContent = jQuery("#i02_c");
	const iUICustomizationContent = jQuery("#i03_c");
	const iEditingSongAttrContent = jQuery("#i04_c");
	const iEditingSongDataContent = jQuery("#i05_c");
	const iSongPlaybackContent = jQuery("#i06_c");
	const iDownloadUploadContent = jQuery("#i07_c");
	// Extraneous
	const iContentViewLabel = jQuery("#iContentViewLabel");
	const iContentExitLink = jQuery("#ix0");
	

	/** Various FontSizes the user can select. */
	const fontSizes = ["10px", "12px", "16px", "20px", "24px"];
	/** The individual jQuery elements whcih represent the hidden text content for each submenu in the instructions. (Editing Mode Only) */
	const instructionContents = [iQuickStartContent, iAutoSaveContent, iUICustomizationContent, iEditingSongAttrContent, iEditingSongDataContent, iSongPlaybackContent, iDownloadUploadContent];

	/**
	 * Should debug messages be printed to the console?
	 * Set to 'false' to disable all debug messages.
	 */
	const printDebug = true;
	const PAUSE_DELAY = 500; // in ms

	/** If the song is currently playing. */
	let isSongPlaying = false;
    /** If the song is currently paused. */
	let isPaused = false;
    /** A track of the current tone/note the song is playing. */
	let currentTone = 0;
	let timePassedSinceLastNote = 0;
	let timePassedSinceSongStart = 0;
	// UI Elements
	/** Keep track of which index of the font size array the user is on. */
	let fontSizeIndex = 2;
	/** If dark mode is enabled. */
	let darkMode = false;

	/**
	 * All of the allowed characters when naming a song.
	 */
	const AllowedNamingChars = Object.freeze(["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "_"]);
	
    // An enum of all possible notes
	const ProgramNotes = Object.freeze({
		"END": 0,
		"PS": 1,
		"B0": 31,
		"C1": 33,
		"CS1": 35,
		"D1": 37,
		"DS1": 39,
		"E1": 41,
		"F1": 44,
		"FS1": 46,
		"G1": 49,
		"GS1": 52,
		"A1": 55,
		"AS1": 58,
		"B1": 62,
		"C2": 65,
		"CS2": 69,
		"D2": 73,
		"DS2": 78,
		"FS2": 93,
		"G2": 98,
		"GS2": 104,
		"A2": 110,
		"AS2": 117,
		"B2": 123,
		"C3": 131,
		"CS3": 139,
		"D3": 147,
		"DS3": 156,
		"E3": 165,
		"F3": 175,
		"FS3": 185,
		"G3": 196,
		"GS3": 208,
		"A3": 220,
		"AS3": 223,
		"B3": 247,
		"C4": 262,
		"CS4": 277,
		"D4": 294,
		"DS4": 311,
		"E4": 330,
		"F4": 349,
		"FS4": 370,
		"G4": 392,
		"GS4": 415,
		"A4": 440,
		"AS4": 466,
		"B4": 494,
		"C5": 523,
		"CS5": 554,
		"D5": 587,
		"DS5": 622,
		"E5": 659,
		"F5": 698,
		"FS5": 740,
		"G5": 784,
		"GS5": 831,
		"A5": 880,
		"AS5": 932,
		"B5": 988,
		"C6": 1047,
		"CS6": 1109,
		"D6": 1175,
		"DS6": 1245,
		"E6": 1319,
		"F6": 1397,
		"FS6": 1480,
		"G6": 1568,
		"GS6": 1661,
		"A6": 1760,
		"AS6": 1865,
		"B6": 1976,
		"C7": 2093,
		"CS7": 2217,
		"D7": 2349,
		"DS7": 2489,
		"E7": 2637,
		"F7": 2794,
		"FS7": 2960,
		"G7": 3136,
		"GS7": 3322,
		"A7": 3520,
		"AS7": 3729,
		"B7": 3951,
		
        /**
         * @param {Number} freq The frequency to get the pitch of.
         * @returns The string representation of a pitch. (Ex: 2794 -> "F7")
         */
		getPitch: (freq) => {
			return Object.keys(ProgramNotes).find(k => ProgramNotes[k] === freq);
		}
	}, );

	/** 
     * Object which represents a song.
     * This objects data should be overwriten/changed depending on the actions the user takes.
     */
	let songContainer = {
		fileName: "EMPTY",
		size: 0,
		attributes: {
			toneLength: 50,
			toneDelay: 80,
			author: "EMPTY", // Who created the song.
			creationDate: getDateAsString(), // Day the song was started.
			lastEdit: "EMPTY" // Last time the song was edited.
		},
		data: {
			src: [ProgramNotes.B0, ProgramNotes.END]
		}
	};

	// Custom prototype array insert function.
	Array.prototype.insert = function(index, item) {
		this.splice(index, 0, item);
	};
	/**
	 * A custom debugging function which prints to the console.
	 * @param {String} str 
	 * @return A debug formatted str.
	 */
	function debug(str) {
		if (!printDebug) return;
		console.log(`[${getDateAsString()}] DEBUG: ${str}`);
	}
	/**
	 * Refreshes the current page and updates the selection mode or editing mode.
	 */
	function refresh() {
		debug("Handling refresh()");
		// Set elements invisible by default.
		selectSongModeTag.css({
			"display": "none"
		});
		editSongModeTag.css({
			"display": "none"
		});
		// Make sure autosave is set and valid.
		if (localStorage.getItem(autoSaveEnabledKey) === "true") {
			if (!isNormalInteger(localStorage.getItem(autoSaveIntervalKey))) {
				localStorage.setItem(autoSaveIntervalKey, autoSaveIntervalDefault);
				debug("Use has auto save enabled but an invalid interval. Set to default.");
			}
		} else {
			if (localStorage.getItem(autoSaveEnabledKey) === null) {
				debug("Null value found for autosave, setting defaults");
				localStorage.setItem(autoSaveEnabledKey, true);
				localStorage.setItem(autoSaveIntervalKey, autoSaveIntervalDefault);
			}
		}
		if (isNormalInteger(localStorage.getItem(autoSaveIntervalKey))) {
			autoSaveSettingsInterval.text(localStorage.getItem(autoSaveIntervalKey));
		} else {
			localStorage.setItem(autoSaveIntervalKey, autoSaveIntervalDefault);
			autoSaveSettingsInterval.text(localStorage.getItem(autoSaveIntervalKey));
		}
		// change the values of the auto save setting fields.
		if (localStorage.getItem(autoSaveEnabledKey) === "true") {
			autoSaveSettingsEnabledCheckbox.prop("checked", true);
		} else {
			autoSaveSettingsEnabledCheckbox.prop("checked", false);
		}
		if(localStorage.getItem(darkModeKey) === "true") {
			darkMode = true;
		}else if(localStorage.getItem(darkModeKey) === "false") {
			darkMode = false;
		}else{
			localStorage.setItem(darkModeKey, false);
			darkMode = false;
		}
		setTheme();


		if(localStorage.getItem(fontTypeKey) === null) {
			localStorage.setItem(fontTypeKey, 0);
		}
		switch(Number(localStorage.getItem(fontTypeKey))) {
			case 1:
				$("body").css("font-family", "Trebuchet MS");
				break;
			case 2:
				$("body").css("font-family", "Times New Roman");
				break;
			case 3:
				$("body").css("font-family", "Arial");
				break;
			case 4:
				$("body").css("font-family", "Verdana");
				break;
			case 5:
				$("body").css("font-family", "Segoe UI");
				break;
		}
		$(changeFontLink).text("Change Font (Current: " + $("body").css("font-family") + ")");

		// Update mode data.
		if (sessionStorage.getItem(currentModeKey) === selectSongModeSS) {
			selectSongModeTag.css({
				"display": "block"
			});
			if (localStorage.getItem(songObjectKey) === null || localStorage.getItem(songObjectKey) === "") {
				debug("Could not find song object.");
				resumeFileButton.css("display", "none");
			}
			userInstructionsSelectMode.css("display", "unset");
			userInstructionsEditingMode.css("display", "none");
		} else if (sessionStorage.getItem(currentModeKey) === editSongModeSS) {
			$("footer").css("display", "unset");
			editSongModeTag.css({
				"display": "block"
			});
			if (localStorage.getItem(songObjectKey) !== "" && localStorage.getItem(songObjectKey) !== "async-file") {
				debug("Found song in localStorage, parsing...");
				songContainer = JSON.parse(atob(localStorage.getItem(songObjectKey)));
			}
			// Update all of the text.
			attrSongName.text(songContainer.fileName);
			attrSongSize.text(songContainer.size);
			attrAuthor.text(songContainer.attributes.author);
			attrToneLength.text(songContainer.attributes.toneLength);
			attrToneDelay.text(songContainer.attributes.toneDelay);
			attrCreationDate.text(songContainer.attributes.creationDate);
			attrLastEditDate.text(songContainer.attributes.lastEdit);
			update();
			userInstructionsSelectMode.css("display", "none");
			userInstructionsEditingMode.css("display", "unset");
		} else {
			sessionStorage.setItem(currentModeKey, selectSongModeSS);
			refresh();
		}
		$(changeFontLink).text("Change Font (Current: " + $("body").css("font-family") + ")");
	}
	/**
	 * Converts the current song object into a JSON string and saves it in local storage.
	 */
	function saveSongObject() {
		if (sessionStorage.getItem(currentModeKey) !== editSongModeSS) {
			return;
		}
        // Because the jQuery text elements are sometimes updated instead of the song object
        // we need to make sure that the song object values and the values of the jQuery elements are the same.
		songContainer.fileName = attrSongName.text().toUpperCase();
		songContainer.size = songContainer.data.src.length - 1; //-1 for "END" note.
		songContainer.attributes.author = attrAuthor.text();
		songContainer.attributes.toneLength = Number(attrToneLength.text());
		songContainer.attributes.toneDelay = Number(attrToneDelay.text());
		// We do not need to update creationDate or lastEdit as those are already updated automatically.
		localStorage.setItem(songObjectKey, btoa(JSON.stringify(songContainer)));
		debug("Song object was saved to localStorage.");
	}
	/**
	 * Updates the last edit for the song.
	 */
	function updateLastEdit() {
		songContainer.attributes.lastEdit = getDateAsString();
	}
	/**
	 * @param {Number} dataIndex The index of the table for the data-command attribute.
	 * @param {String} selectedPitch The pitch to select in the table.
	 * @returns A HTML element <select> table with all possible frequencies. If the selectedPitch parameter matches any frequency then it will become the selected element.
	 */
	function getPitchSelectionTableHTML(dataIndex, selectedPitch) {
		let tableHTML = `<select data-command="edit,` + dataIndex + `" title="Select Pitch">`;
		// i=1 to skip END, .length-1 to skip getPitch() method.
		for (let i = 1; i < Object.keys(ProgramNotes).length - 1; i++) {
			if (Object.keys(ProgramNotes)[i] === selectedPitch) {
				tableHTML += `<option value="` + Object.keys(ProgramNotes)[i] + `" selected>` + Object.keys(ProgramNotes)[i] + `</option>`;
				continue;
			}
			tableHTML += `<option value="` + Object.keys(ProgramNotes)[i] + `">` + Object.keys(ProgramNotes)[i] + `</option>`;
		}
		tableHTML += `</select>`;
		return tableHTML;
	}
	/**
	 * Converts an index, pitch, and frequency to an html row which can be added to an html table.
	 * @param {Number} index The index of the table to insert at.
	 * @param {String} pitch The pitch of the note.
	 * @param {Number} frequency The frequency of the note.
	 */
	function fillHTMLTableRow(index, pitch, frequency) {
		// A HTML representation of a row.
		let rowData = `<tr> <td style="padding: 8px;"><strong>{{INDEX}}</strong></td> <td>{{PITCH}}</td> <td>{{FREQUENCY}}</td> <td> <table border="0" cellspacing="0" cellpadding="2"> <tr> <td> <button aria-label="add" data-command="add,{{INDEX}}" style="padding: 5px; margin: 0; background: none;"> <i class="fas fa-plus-circle fa-lg" style="color: green;"></i> </button> </td> <td> <button aria-label="rem" data-command="rem,{{INDEX}}" style="padding: 5px; margin: 0; background: none;"> <i class="fas fa-trash-alt fa-lg" style="color: red;"></i> </button> </td> </tr> </table> </td> </tr>`;
		if (frequency === ProgramNotes.END) {
			rowData = `<tr> <td style="padding: 8px;"><strong>{{INDEX}}</strong></td> <td>{{PITCH}}</td> <td>{{FREQUENCY}}</td> <td> <table border="0" cellspacing="0" cellpadding="2"> <tr> <td> <button aria-label="add" style="padding: 5px; margin: 0; background: none;" disabled> <i class="fas fa-plus-circle fa-lg" style="color: green;"></i> </button> </td> <td> <button aria-label="rem" style="padding: 5px; margin: 0; background: none;" disabled> <i class="fas fa-trash-alt fa-lg" style="color: red;"></i> </button> </td> </tr> </table> </td> </tr>`;
		}
		// Replace the placeholders.
		rowData = rowData.split("{{INDEX}}").join(String(index));
		rowData = rowData.split("{{PITCH}}").join(String(pitch));
		rowData = rowData.split("{{FREQUENCY}}").join(String(frequency));
		return rowData;
	}
	/**
	 * Updates the current table of notes on the website as well as some of the text that is related to the song.
	 * Clears the child elements of the table and parses each note in the song arraylist and adds it.
     * Adds clicking events to the buttons of each tone (add/rem) as well as handles an event when the dropbox pitch is changed (updates frequency).
	 */
	function update() {
		// Delete all child elements.
		songDataTable.empty();
		// Defines HTML rows for the table.
		songDataTable.append("<tr> <th>#</th> <th>Pitch</th> <th>Frequency (Hz)</th> <th>Action</th> </tr>");
		for (let i = 0; i < songContainer.data.src.length; i++) {
			if (i === (songContainer.data.src.length - 1)) {
				// Make the last note red.
				songDataTable.append(fillHTMLTableRow(i + 1, `<span style='font-weight: bold; color: red;'>${ProgramNotes.getPitch(songContainer.data.src[i])}</span>`, songContainer.data.src[i]));
				continue;
			}
			songDataTable.append(fillHTMLTableRow(i + 1, getPitchSelectionTableHTML(i + 1, ProgramNotes.getPitch(songContainer.data.src[i])), songContainer.data.src[i]));
		}
		// -1 for ending note.
		songContainer.size = songContainer.data.src.length - 1;
		// Update the text.
		attrSongSize.text(songContainer.size);
		attrLastEditDate.text(songContainer.attributes.lastEdit);
		// Reinstantiate the event which checks when a add/remove button is clicked on a specific row. This must be done every time the table is updated to
		// keep the javascript insync with the DOM.
		// CHECK IF A ADD/REMOVE BUTTON ON A TABLE WAS CLICKED
		$("button").click((e) => {
			let element;
			// We must check if the <i> part of the button is being clicked.
			if ($(e.target).is("i")) {
				element = $(e.target).parent().data('command');
			} else {
				element = $(e.target).data('command');
			}
			if (element === undefined) return;
			if (element.split(",")[0] === "add") {
				let index = Number(element.split(",")[1]);
				songContainer.data.src.insert(index, songContainer.data.src[index-1]);
				updateLastEdit();
				update();
			} else if (element.split(",")[0] === "rem") {
				let index = Number(element.split(",")[1]);
				if (songContainer.size === 1) { // End Note + Song Note
					alert("You cannot remove your last note!\nEither edit this note or add more!");
					return;
				}
				songContainer.data.src.splice(index - 1, 1);
				updateLastEdit();
				update();
			} else {
				debug("ERROR: Found data-command tag but not a proper command.");
			}
		});
		// GET WHEN A ELEMENT IS CHANGED USING THE DROPDOWN BOX AND UPDATE THE SONG OBJECT.
		$("select").change((e) => {
			// Ensure that the select tag we are opening is to be edited.
			if ($(e.target).data("command").split(",")[0] === "edit") {
				// Replace the index in the songContainer notes list with the new note the user selected.
				songContainer.data.src[Number($(e.target).data("command").split(",")[1]) - 1] = Object.values(ProgramNotes)[Object.keys(ProgramNotes).indexOf($(e.target).val())];
				//saveSongObject();
				updateLastEdit();
				update(); // Refresh the table.
			}
		});
		// Update font sizes.
		songDataTable.css("font-size", fontSizes[fontSizeIndex]);
		$("select").css("font-size", fontSizes[fontSizeIndex]);
		setTheme();
		debug("Finished updating table");
	}
	/**
	 * Returns the user back to selection mode.
	 */
	function returnSelectMode() {
		location.reload();
		sessionStorage.setItem(currentModeKey, selectSongModeSS);
		refresh();
	}
	/**
	 * Returns if a string is valid to be saved. (A valid save name)
	 * Note that this method does not check lengths.
	 * @param {String} character to check (char code)
	 * @return A boolean if the string could be saved. 
	 */
	function isValidNamingChar(str) {
		if (!AllowedNamingChars.includes(String.fromCharCode(str).toUpperCase())) {
			return false;
		}
		return true;
	}

	/**
	 * Sets the theme of the application depending on the value of the "darkMode" variable.
	 */
	function setTheme() {
		if(darkMode) {
			toggleDarkModeLink.text("Toggle Dark Mode (Dark Mode: On)");
			$("body").css("background-color", "#212226");
			$("body").css("color", "white");
			$("span").css("border-color", "white");
			$("button").css("background-color", "#3b4254");
		}else{
			toggleDarkModeLink.text("Toggle Dark Mode (Dark Mode: Off)");
			$("body").css("background-color", "unset");
			$("body").css("color", "unset");
			$("span").css("border-color", "unset");
			$("button").css("background-color", "unset");
		}
	}

	/**
	 * @return The current date made in a custom format.
	 */
	function getDateAsString() {
		const date = new Date();
		let dateAsString = "";
		// Parse the month
		if (date.getMonth() + 1 < 10) {
			dateAsString += "0";
			dateAsString += date.getMonth() + 1;
		} else {
			dateAsString += date.getMonth() + 1;
		}
		// Add date seperator.
		dateAsString += "/";
		// Pase the day
		if (date.getDate() < 10) {
			dateAsString += "0";
			dateAsString += date.getDate();
		} else {
			dateAsString += date.getDate();
		}
		// Add date seperator.
		dateAsString += "/";
		// Add year
		dateAsString += date.getFullYear();
		// Add date/time seperator.
		dateAsString += " ~ ";
		// Add time
		if (date.getHours() > 12) {
			dateAsString += date.getHours() - 12;
		} else {
			dateAsString += date.getHours();
		}
		dateAsString += ":";
		if (date.getMinutes() < 10) {
			dateAsString += "0";
			dateAsString += date.getMinutes();
		} else {
			dateAsString += date.getMinutes();
		}
		dateAsString += " ";
		if (date.getHours() <= 12) {
			dateAsString += "AM";
		} else {
			dateAsString += "PM";
		}
		//dateAsString += date.getHours();
		return dateAsString;
	}
	/**
	 * 
	 * @param {String} str A string to test
	 * @returns If a string is only valid integers (positive).
	 */
	function isNormalInteger(str) {
		var n = Math.floor(Number(str));
		return n !== Infinity && String(n) === str && n >= 0;
	}
	// Global audio variable to play sound.
	const audioCtx = new(window.AudioContext || window.webkitAudioContext || window.audioContext);
	/**
	 * Makes a beeping sound with the given parameters.
	 * @param {Number} duration The number of ms to play the tone for.
	 * @param {Number} frequency The hz frequency.
	 * @param {Number} volume Between 0 and 1 (FP numbers allowed) 
	 * @param {String} type Should always be "square"
	 * @param {*} callback A callback function to divert to.
	 */
	function beep(duration, frequency, volume, type, callback) {
		const oscillator = audioCtx.createOscillator();
		const gainNode = audioCtx.createGain();
		oscillator.connect(gainNode);
		gainNode.connect(audioCtx.destination);
		if (volume) {
			gainNode.gain.value = volume;
		}
		if (frequency) {
			oscillator.frequency.value = frequency;
		}
		if (type) {
			oscillator.type = type;
		}
		if (callback) {
			oscillator.onended = callback;
		}
		oscillator.start(audioCtx.currentTime);
		oscillator.stop(audioCtx.currentTime + ((duration || 500) / 1000));
	};
	/**
	 * @return The current song object parsed as text which can be read by TuneStudio2560.
	 */
	function songToText() {
		let file = "";
		file += 
`# Welcome to a song file!
# This song was created using MakerStudio2560, a custom song creation and playback tool made for the TuneStudio2560! (https://devjluvisi.github.io/TuneStudio2560/MakerStudio2560)

Song Author: ${songContainer.attributes.author}
Creation Date: ${songContainer.attributes.creationDate}
Last Edited: ${songContainer.attributes.lastEdit}

# To view more information, check out https://github.com/devjluvisi/TuneStudio2560/wiki/For-Users

# The delay between each different tone (ms). (Must be 9999 or less and greater than 0)
TONE_DELAY=${songContainer.attributes.toneDelay}

# The length that each tone should play for (ms). (Must be 255 or less and greater than 0)
TONE_LENGTH=${songContainer.attributes.toneLength}

Data:\n`;
		for (let i = 0; i < songContainer.size; i++) {
			file += `  - ` + ProgramNotes.getPitch(songContainer.data.src[i]) + `\n`;
		}
		file += `\n`;
		file += `# END`;
		return file;
	}
	/**
	 * Downloads a file using a file name and text.
	 * 
	 * @param {String} filename The filename to save as.
	 * @param {String} text The text to put in the file
	 */
	function download(filename, text) {
		const element = document.createElement('a');
		element.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(text));
		element.setAttribute('download', filename);
		element.style.display = 'none';
		document.body.appendChild(element);
		element.click();
		document.body.removeChild(element);
	}
	/**
	 * Read the text from a file.
	 * @param {*} evt 
	 * @return The raw text from a file.
	 */
	function readFile(evt, onloadcallback) {
		let files = evt.target.files;
		let file = files[0];
		let reader = new FileReader();
		reader.onload = onloadcallback;
		reader.readAsText(file);
	}

	function getAllIndexes(arr, val) {
		var indexes = [], i;
		for(i = 0; i < arr.length; i++)
			if (arr[i] === val)
				indexes.push(i);
		return indexes;
	}

	/**
	 * @returns A (rough estimation) of the current time the user is in on the song. (In Seconds)
	 */
	function calculateCurrentSeconds() {
		let seconds = 0;
		for(let i = 0; i < currentTone; i++) {
			if(songContainer.data.src[i] != ProgramNotes.PS) {
				seconds += songContainer.attributes.toneDelay + songContainer.attributes.toneLength;
				continue;
			}
			seconds += songContainer.attributes.toneDelay + songContainer.attributes.toneLength + PAUSE_DELAY;
		}
		return Math.trunc((seconds + (songContainer.attributes.toneDelay + songContainer.attributes.toneLength) ) /1000);
		//return Math.abs((calculateMaxSeconds() - Math.trunc(timePassedSinceSongStart/1000)) - calculateMaxSeconds());
	}

	/**
	 * @returns A (rough estimation) of the maximum amount of time the song will go for depending on the amount of notes and pauses in it.
	 */
	function calculateMaxSeconds() {
		let pauseTime;
		let toneDelayTime;
		let toneLengthTime;
		
		pauseTime = getAllIndexes(songContainer.data.src, 1).length * PAUSE_DELAY;
		toneDelayTime = songContainer.attributes.toneDelay * songContainer.size;
		toneLengthTime = songContainer.attributes.toneLength * songContainer.size;

		return Math.trunc(((pauseTime + toneDelayTime + toneLengthTime) + (songContainer.attributes.toneDelay + songContainer.attributes.toneLength) ) / 1000);

	}

	/**
	 * @returns If enough time has passed that we must play the next note.
	 */
	function isAllowedToPlayNextNote() {
		if(timePassedSinceLastNote > 0) timePassedSinceLastNote-=5;
		if(timePassedSinceLastNote <= 0) timePassedSinceLastNote = 0;
		return timePassedSinceLastNote <= 0;
	}

	// General Events.
	resumeFileButton.click(() => {
		debug("Attempting to resume edit");
		sessionStorage.setItem(currentModeKey, editSongModeSS);
		refresh();
	});
	newFileButton.click(() => {
		debug("Deleted data from localStorage");
		sessionStorage.setItem(currentModeKey, editSongModeSS);
		localStorage.setItem(songObjectKey, "");
		// Reset the values of songContainer to defaults (useful when a user selects a file but then selects "Create New")
		songContainer = {
			fileName: "EMPTY",
			size: 0,
			attributes: {
				toneLength: 50,
				toneDelay: 80,
				author: "EMPTY", // Who created the song.
				creationDate: getDateAsString(), // Day the song was started.
				lastEdit: "EMPTY" // Last time the song was edited.
			},
			data: {
				src: [ProgramNotes.B0, ProgramNotes.END]
			}
		};
		refresh();
	});
	getFileButton.change((e) => {
		debug("Reading from selected file...");
		sessionStorage.setItem(currentModeKey, editSongModeSS);
		
		// After we get info from the file, we read each line and then add the data to the song.
		// The following is very similar to the code used directly in TuneStudio2560.
		readFile(e, function(ev) {
			let result = ev.target.result;
			// Set name.
			let songName = e.target.files[0].name.substring(0, e.target.files[0].name.length-4).trim(); // REMOVE .TXT EXTENSION
			for(let i = 0; i < songName.length; i++) {
				if(!isValidNamingChar(songName.charCodeAt(i))) {
					alert("-- INVALID FILE --\nYour file was rejected due to one or more reasons:\n- File name is bad (8 char max, A-Z, 0-9 and underscores only).");
					sessionStorage.setItem(currentModeKey, selectSongModeSS);
					return;
				}
			}
			attrSongName.text(songName);
			
			
			// Track if we are setting the tone delay or tone length.
			let isToneDelay = true;
			// Track if we are reading the Song Author, Creation Date, or last edit date.
			let metaDataKey = 0;

			// Track which note we have added.
			let noteIndex = 0;

			// Remove the "B0" starting note.
			songContainer.data.src = [ProgramNotes.END];
			for(let i = 0; i < result.length; i++) {
				// Ignore Comments
				if(result.charAt(i) === '#') {
					while(result.charAt(i) !== '\n' && i < result.length) {
						i++;
					}
				}
				// Check for meta data like author, etc.
				if(result.charAt(i) === ':' && metaDataKey <= 2) {
					let mdata = "";
					while(result.charAt(i) !== '\n') {
						i++;
						mdata += result.charAt(i);
					}
					if(mdata === "" || mdata === " " || mdata === "\n") continue;
					if(metaDataKey === 0) {
						attrAuthor.text(mdata.trim());
						songContainer.attributes.author = mdata.trim();
					}else if(metaDataKey === 1) {
						songContainer.attributes.creationDate = mdata.trim();
					}else if(metaDataKey === 2) {
						songContainer.attributes.lastEdit = mdata.trim();
					}
					metaDataKey++;
				}
				// Check for tone length, tone delay
				if(result.charAt(i) === '=') {
					if(isToneDelay) {
						let toneDelay = "";
						while(result.charAt(i) !== '\n') {
							i++;
							toneDelay += result.charAt(i);
						}
						isToneDelay = false;
						attrToneDelay.text(toneDelay);
						// Set an invalid tone delay if the tone delay is not a number.
						songContainer.attributes.toneDelay = String(Number(toneDelay)) !== "NaN" ? Number(toneDelay) : -1;
					}else{
						let toneLength = "";
						while(result.charAt(i) !== '\n') {
							i++;
							toneLength += result.charAt(i);
						}
						attrToneLength.text(toneLength);
						// Set an invalid tone length if the tone length is not a number.
						songContainer.attributes.toneLength = String(Number(toneLength)) !== "NaN" != NaN ? Number(toneLength) : -1;
					}
				}
				// Check for pitches.
				if(result.charAt(i) === '-') {
					let pitch = "";
					while(result.charAt(i) !== '\n') {
						i++;
						pitch += result.charAt(i);
					}
					// Insert the frequency at the index of the song before the "END" note.
					songContainer.data.src.insert(noteIndex, Object.values(ProgramNotes)[Object.keys(ProgramNotes).indexOf(pitch.trim())]);
					noteIndex++;
				}
			}
			if(songContainer.attributes.toneDelay > 9999 || songContainer.attributes.toneDelay < 0 || songContainer.attributes.toneLength > 255 || songContainer.attributes.toneLength < 0 || songContainer.attributes.author.length > 15) {
				alert("-- INVALID FILE --\nYour file was rejected due to one or more reasons:\n- Tone Delay OUT OF BOUNDS. Must be between 0 and 9999.\n- Tone Length OUT OF BOUNDS. Must be between 0 and 255\n- Author Length OUT OF BOUNDS. Must be less then 15 characters.");
				sessionStorage.setItem(currentModeKey, selectSongModeSS);
				location.reload();
				return;
			}
			resumeFileButton.css("display", "unset");
			saveSongObject();
			
			alert("File has been parsed.\nClick on \"Resume File\" to edit.");
			
		});
	});
	// Return back to selection screen.
	returnSelectModeLink.click(() => returnSelectMode());
	/// ATTRIBUTE SETTINGS
	attrToneDelay.keypress((e) => {
		if (attrToneDelay.text().length === 4 || !isNormalInteger(String.fromCharCode(e.which))) {
			e.preventDefault();
			e.stopImmediatePropagation();
		}
	});
	attrToneLength.keypress((e) => {
		if (attrToneLength.text().length === 3 || !isNormalInteger(String.fromCharCode(e.which))) {
			e.preventDefault();
			e.stopImmediatePropagation();
			return;
		}
	});
	attrToneLength.keyup(() => {
        // "255" max value of toneLength
		if (Number(attrToneLength.text()) > 255) {
			attrToneLength.text("255");
		}
	});
	attrAuthor.keypress((e) => {
		if (attrAuthor.text().length === 15 || e.which === "-".charCodeAt(0) || e.which === "=".charCodeAt(0)) {
			e.preventDefault();
			e.stopImmediatePropagation();
		}
	});
	attrSongName.keypress((e) => {
		// Limit size of string, ensure valid letters.
		if (attrSongName.text().length === 8 || !isValidNamingChar(e.which)) {
			e.preventDefault();
			e.stopImmediatePropagation();
		}
	});
	/// AUTOSAVE SETTINGS
	autoSaveSettingsInterval.keypress((e) => {
		if (autoSaveSettingsInterval.text().length === 3) {
			e.preventDefault();
			e.stopImmediatePropagation();
		}
	});
	autoSaveMessageSummaryHeader.mouseenter(() => {
		if (localStorage.getItem(autoSaveEnabledKey) === 'true') {
			autoSaveMessage.text("Your progress is automatically saved in your browser when you exit. (Click to Configure)");
		} else {
			autoSaveMessage.text("Your progress will not save (auto save disabled). (Click to Configure)");
		}
	});
	autoSaveMessageSummaryHeader.mouseleave(() => {
		if (localStorage.getItem(autoSaveEnabledKey) === 'true') {
			autoSaveMessage.text("Your progress is automatically saved in your browser when you exit.");
		} else {
			autoSaveMessage.text("Your progress will not save (auto save disabled).");
		}
	});
	autoSaveSettingsSaveLink.click(() => {
		if (isNormalInteger(autoSaveSettingsInterval.text()) && Number(autoSaveSettingsInterval.text()) > 0) {
			localStorage.setItem(autoSaveIntervalKey, autoSaveSettingsInterval.text());
		} else {
			alert("Invalid settings for autosave.\nPlease use numbers between 1 and 999.");
			return;
		}
		localStorage.setItem(autoSaveEnabledKey, autoSaveSettingsEnabledCheckbox.prop("checked"));
		alert("Your auto save settings have been saved.");
		saveSongObject();
		location.reload();
	});
	// [Media Playback] Pause/Play Buttons
	playPauseSongPlaybackButton.click(() => {
		if (currentTone === songContainer.size) {
			return;
		}
		if (!isSongPlaying) {
			isSongPlaying = true;
			isPaused = false;
			playPauseSongPlaybackButton.attr("class", "fas fa-pause-circle fa-lg");
			songPlaybackStatusLabel.text("Now Playing: " + songContainer.fileName);
		} else {
			if (isPaused) {
				songPlaybackStatusLabel.text("Now Playing: " + songContainer.fileName);
				playPauseSongPlaybackButton.attr("class", "fas fa-pause-circle fa-lg");
				isPaused = false;
			} else {
				songPlaybackStatusLabel.text("Now Paused: " + songContainer.fileName);
				playPauseSongPlaybackButton.attr("class", "fas fa-play-circle fa-lg");
				isPaused = true;
			}
		}
	});
	// [Media Playback] Restart the current song.
	restartSongPlaybackButton.click(() => {
		timePassedSinceSongStart = 0;
		currentTone = 0;
		isPaused = false;
		isSongPlaying = true;
		songPlaybackStatusLabel.text("Now Playing: " + songContainer.fileName);
		playPauseSongPlaybackButton.attr("class", "fas fa-pause-circle fa-lg");
	});
	// [Media Playback] Stop the current song.
	stopSongPlaybackButton.click(() => {
		timePassedSinceSongStart = 0;
		currentTone = 0;
		isSongPlaying = false;
		isPaused = false;
		playPauseSongPlaybackButton.attr("class", "fas fa-play-circle fa-lg");
		songPlaybackStatusLabel.text("Press Play to Start Song");
	});
	// Media Playback Skip Forward by 10%
	skipForwardSongPlaybackButton.click(() => {
		if (!isSongPlaying) return;
		currentTone += Math.trunc(0.1 * songContainer.size < 1 ? 1 : 0.1 * songContainer.size);
		if (currentTone > songContainer.size) {
			currentTone = songContainer.size;
		}
	});
	// Media Playback Rewind by 10%
	skipBackSongPlaybackButton.click(() => {
		if (!isSongPlaying) return;
		currentTone -= Math.trunc(0.1 * songContainer.size < 1 ? 1 : 0.1 * songContainer.size);
		if (currentTone < 0) {
			currentTone = 0;
		}
	});
	// Restart the song (delete all saved data)
	restartSongButton.click(() => {
		if (prompt("Enter 'Y' if you are sure you want to delete all saved data.") === "Y") {
			songContainer = null;
			localStorage.setItem(songObjectKey, "");
			location.reload();
		}
	});

	// Download the song button.
	downloadSongButton.click(() => download(songContainer.fileName, songToText()));

    // Copy the song to the clipboard button.
	copySongDataButton.click(() => {
		//async clipboard api
		navigator.clipboard.writeText(songToText()).then(() => {
			alert("Text for Song Copied.\nPaste using CTRL+V");
		}).catch(() => {
			alert("There was a problem. Could not copy to clipboard.");
		});
	});

	

	// UI Change Buttons
	fontIncreaseButton.click(() => {
		if(fontSizeIndex === fontSizes.length-1) {
			return;
		}
		fontSizeIndex++;
		songDataTable.css("font-size", fontSizes[fontSizeIndex]);
		$("select").css("font-size", fontSizes[fontSizeIndex]);
	});

	fontDecreaseButton.click(() => {
		if(fontSizeIndex === 0) {
			return;
		}
		fontSizeIndex--;
		songDataTable.css("font-size", fontSizes[fontSizeIndex]);
		$("select").css("font-size", fontSizes[fontSizeIndex]);
	});

	toggleDarkModeLink.click(() => {
		if(darkMode) {
			darkMode = false;
			localStorage.setItem(darkModeKey, false);
		}else{
			darkMode = true;
			localStorage.setItem(darkModeKey, true);
		}
		setTheme();
	});

	changeFontLink.click(() => {
		
		let fontResponse = prompt("Select a font by typing in a number.\nThe font you select will be saved when you exit.\n(1) Trebuchet MS (Default)\n(2) Times New Roman\n(3) Arial\n(4) Verdana\n(5) Segoe UI");
		if(fontResponse === null) return;
		if(isNormalInteger(fontResponse) && Number(fontResponse) > 0 && Number(fontResponse) <= 5) {
			switch(Number(fontResponse)) {
				case 1:
					$("body").css("font-family", "Trebuchet MS");
					break;
				case 2:
					$("body").css("font-family", "Times New Roman");
					break;
				case 3:
					$("body").css("font-family", "Arial");
					break;
				case 4:
					$("body").css("font-family", "Verdana");
					break;
				case 5:
					$("body").css("font-family", "Segoe UI");
					break;
			}
			localStorage.setItem(fontTypeKey, Number(fontResponse));
			$(changeFontLink).text("Change Font (Current: " + $("body").css("font-family") + ")");
		}else{
			alert("Invalid Input.\nPlease try again.");
		}
		
	});
	/// FOR: INSTRUCTIONS DETAILS
	iContentExitLink.click(() => {
		editingModeInstructionContentMenu.css("display", "unset");
		editingModeInstructionsContent.css("display", "none");
		instructionContents.forEach(e => e.css("display", "none"));
	});
	iQuickStartLink.click(() => {
		editingModeInstructionContentMenu.css("display", "none");
		editingModeInstructionsContent.css("display", "unset");

		iQuickStartContent.css("display", "unset");
		iContentViewLabel.text("Quick Start");
	});
	iAutoSaveLink.click(() => {
		editingModeInstructionContentMenu.css("display", "none");
		editingModeInstructionsContent.css("display", "unset");

		iAutoSaveContent.css("display", "unset");
		iContentViewLabel.text("Auto Save");
	});
	iUICustomizationLink.click(() => {
		editingModeInstructionContentMenu.css("display", "none");
		editingModeInstructionsContent.css("display", "unset");

		iUICustomizationContent.css("display", "unset");
		iContentViewLabel.text("UI Customization");
	});
	iEditingSongAttrLink.click(() => {
		editingModeInstructionContentMenu.css("display", "none");
		editingModeInstructionsContent.css("display", "unset");

		iEditingSongAttrContent.css("display", "unset");
		iContentViewLabel.text("Editing Song Attributes");
	});
	iEditingSongDataLink.click(() => {
		editingModeInstructionContentMenu.css("display", "none");
		editingModeInstructionsContent.css("display", "unset");

		iEditingSongDataContent.css("display", "unset");
		iContentViewLabel.text("Editing Song Data");
	});
	iSongPlaybackLink.click(() => {
		editingModeInstructionContentMenu.css("display", "none");
		editingModeInstructionsContent.css("display", "unset");

		iSongPlaybackContent.css("display", "unset");
		iContentViewLabel.text("Song Playback");
	});
	iDownloadUploadLink.click(() => {
		editingModeInstructionContentMenu.css("display", "none");
		editingModeInstructionsContent.css("display", "unset");

		iDownloadUploadContent.css("display", "unset");
		iContentViewLabel.text("Downloading & Uploading");
	});

	/*
	-- RUNTIME EXECUTION --
	Code to be run when the webpage first loads.
	*/
	$(document).ready(() => {
        // Update the view.
		refresh();
		if (songContainer.attributes.creationDate === "EMPTY") {
			attrCreationDate.text(getDateAsString());
			songContainer.attributes.creationDate = getDateAsString();
			refresh();
		}
		// If auto save enabled
		if (localStorage.getItem(autoSaveEnabledKey) === 'true') {
			setInterval(() => {
				saveSongObject();
				// Save the song at the user-defined interval.
			}, Number(localStorage.getItem(autoSaveIntervalKey)) * 1000);
			debug("Enabled auto save. Set interval to " + (Number(localStorage.getItem(autoSaveIntervalKey)) * 1000));
		} else {
			autoSaveMessage.text("Your progress will not save (auto save disabled).");
		}
        
		// Set an interval for playing the song
		setInterval(() => {
			// Only play a note if these parameters are true.
			if (!(currentTone >= songContainer.size || !isSongPlaying || isPaused || !isAllowedToPlayNextNote())) {
                if(songContainer.data.src[currentTone] != ProgramNotes.PS) {
                    beep(songContainer.attributes.toneLength, songContainer.data.src[currentTone], songVolumeSlider.val() / 100, "square");
                }else{
					timePassedSinceLastNote += PAUSE_DELAY;
				}
				timePassedSinceLastNote += songContainer.attributes.toneDelay + songContainer.attributes.toneLength;
				timePassedSinceSongStart += timePassedSinceLastNote;
				currentTone++;

				/// ALWAYS UPDATE THE FOLLOWING
			// Calculate Current Time
			let secondsPassed = calculateCurrentSeconds();
			let minutesPassed = Math.trunc(secondsPassed / 60);
            secondsPassed -= 60 * minutesPassed;

			if (secondsPassed < 10) {
				currentSongProgressTime.text(minutesPassed + ":" + "0" + secondsPassed);
			} else {
				currentSongProgressTime.text(minutesPassed + ":" + secondsPassed);
			}
			// Calculate Time
			let maxSecondsPassed = calculateMaxSeconds();
			let maxMinutesPassed = Math.trunc(maxSecondsPassed / 60);
            maxSecondsPassed -= 60 * maxMinutesPassed;

			if (maxSecondsPassed < 10) {
				endingSongProgressTime.text(maxMinutesPassed + ":" + "0" + maxSecondsPassed);
			} else {
				endingSongProgressTime.text(maxMinutesPassed + ":" + maxSecondsPassed);
			}
			// Update Progressbar
			songProgressBar.val(calculateCurrentSeconds());
			songProgressBar.attr("max", calculateMaxSeconds());
			// If the song has reached its max then end.
			}
			if (currentTone === songContainer.size) {
				isSongPlaying = false;
				songPlaybackStatusLabel.text("Finished Playing: " + songContainer.fileName);
			}
		}, 4);
	});
	});
})();